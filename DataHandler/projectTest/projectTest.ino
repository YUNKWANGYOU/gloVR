#include "DataHandler.h"
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

DataHandler test(9,8);
uint8_t *flexData;

bool SendButton = false;
bool newData = false;

MPU6050 mpu;

#define INTERRUPT_PIN 2

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity;
float ypr[3];

volatile bool mpuInterrupt = false;

void InitMPU(){
    
  Wire.begin();
  Wire.setClock(400000);

  Serial.begin(115200);

  // initialize device
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(131);
  mpu.setYGyroOffset(-56);
  mpu.setZGyroOffset(-39);
  mpu.setXAccelOffset(-789);
  mpu.setYAccelOffset(-379);
  mpu.setZAccelOffset(910); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
  // turn on the DMP, now that it's ready
  Serial.println(F("Enabling DMP..."));
  mpu.setDMPEnabled(true);

  // enable Arduino interrupt detection
  Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
  mpuIntStatus = mpu.getIntStatus();

  // set our DMP Ready flag so the main loop() function knows it's okay to use it
  Serial.println(F("DMP ready! Waiting for first interrupt..."));
  dmpReady = true;

  // get expected DMP packet size for later comparison
  packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
  // ERROR!
  // 1 = initial memory load failed
  // 2 = DMP configuration updates failed
  // (if it's going to break, usually the code will be 1)
  Serial.print(F("DMP Initialization failed (code "));
  Serial.print(devStatus);
  Serial.println(F(")"));
  }

}


void getYPR() {
 // if programming failed, don't try to do anything
 if (!dmpReady) return;

 // wait for MPU interrupt or extra packet(s) available
 while (!mpuInterrupt && fifoCount < packetSize) {
 // other program behavior stuff here
 // .
 // .
 // .
 // if you are really paranoid you can frequently test in between other
 // stuff to see if mpuInterrupt is true, and if so, "break;" from the
 // while() loop to immediately process the MPU data
 // .
 // .
 // .
 test.RotateServo();
 }

 // reset interrupt flag and get INT_STATUS byte
 mpuInterrupt = false;
 mpuIntStatus = mpu.getIntStatus();

 // get current FIFO count
 fifoCount = mpu.getFIFOCount();

 // check for overflow (this should never happen unless our code is too inefficient)
 if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
 // reset so we can continue cleanly
 test.RotateServo();
 mpu.resetFIFO();
 Serial.println(F("FIFO overflow!"));

 // otherwise, check for DMP data ready interrupt (this should happen frequently)
 } else if (mpuIntStatus & 0x02) {
 // wait for correct available data length, should be a VERY short wait
 while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

 // read a packet from FIFO
 mpu.getFIFOBytes(fifoBuffer, packetSize);
 
 // track FIFO count here in case there is > 1 packet available
 // (this lets us immediately read more without waiting for an interrupt)
 fifoCount -= packetSize;

 // display Euler angles in degrees
 mpu.dmpGetQuaternion(&q, fifoBuffer);
 mpu.dmpGetGravity(&gravity, &q);
 mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
 Serial.print("ypr\t");
 Serial.print(ypr[0] * 180/M_PI,3);
 Serial.print("\t");
 Serial.print(ypr[1] * 180/M_PI,3);
 Serial.print("\t");
 Serial.println(ypr[2] * 180/M_PI,3);
 }
}

void dmpDataReady() {
 mpuInterrupt = true;
}

void setup() 
{
  test.InitFlex();
	test.FilterDeg(0.5);
	test.InitServo();
	test.InitVibe();

  

  InitMPU();
  getYPR();
  Serial.println("Done");
  
} 

void loop() 
{
  test.ReceiveData();

  if(test.newData == true){
    // when recieve program start message, end message, vibration module message
    Serial.print("unityToArduinoDataArray : ");
    Serial.println(test.unityToArduinoDataArray);
    Serial.print("recvLen : ");
    Serial.println(test.recvLen);

    if(test.recvLen == 1){

      //when message is start message
      if(test.unityToArduinoDataArray[0] == '1'){
        SendButton = true;
        test.ClearArr();
      }
      //when message is end message
      else if(test.unityToArduinoDataArray[0] == '3'){
        SendButton = false;
        test.ClearArr();
      }
      else if(test.unityToArduinoDataArray[0] == '2'){
        test.TurnVibeOn();
        test.ClearArr();
      }

    }
    //when servo motor control data, vibe motor control data
    else if(test.recvLen == 5){
      Serial.println("rotate servo");
      test.RotateServo();
      test.ClearArr();
    }

    test.newData = false;
  }

  if(SendButton){
    Serial.println("send data");
    flexData = test.GetFlexData();
    getYPR();
    test.SendData(flexData,ypr);
  }

  test.MakeVibe();

//  //플로터 출력을 위한 임시 코드
//  int data0 = *(flexData+0);
//  int data1 = *(flexData+1);
//  int data2 = *(flexData+2);
//  int data3 = *(flexData+3);
//  int data4 = *(flexData+4);
//  Serial.print(data0);
//  Serial.print(' '); 
//  Serial.print(data1); 
//  Serial.print(' ');
//  Serial.print(data2); 
//  Serial.print(' ');
//  Serial.print(data3); 
//  Serial.print(' ');
//  Serial.println(data4); 
// 
//
//  flexData = test.GetFlexData();
  delay(5);
}
