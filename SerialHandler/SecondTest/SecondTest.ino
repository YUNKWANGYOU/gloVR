#include "SerialHandler.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include <Servo.h>


//define Flex Pin Number
#define flex0Pin A0
#define flex1Pin A1
#define flex2Pin A2
#define flex3Pin A3
#define flex4Pin A6

//define Servo Motor Pin Number
#define servo0Pin 3
#define servo1Pin 4
#define servo2Pin 5
#define servo3Pin 6
#define servo4Pin 7

//define Vibe Motor Pin Number and Vibration Duration
#define vibePin 9
#define vibeDuration 10

//define Zyro Interrupt Pin
#define INTERRUPT_PIN 2

//--------------------------------member--------------------------------------

//Needs To Get Flex Data
float alpha;
uint16_t filteredValue[5];
uint8_t angleValue[5] = {0,};
uint16_t flexValueArr[5];
uint16_t flexMax[5] = { 830, 740, 900, 790, 890 };
uint16_t flexMin[5] ={ 710, 460, 710, 510, 740 };

//Needs To Vibration
uint8_t vibeCount = 0;
bool vibeState = false;

//Needs To Servo Motor
Servo servoArr[5];

//Needs To Zyro Sensor
MPU6050 mpu;

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

//Needs To MainFunc
SerialHandler test(8,9);
bool SendButton = false;
bool newData = false;

//-----------------------------define func----------------------------------
//Init

void InitFlex(){
  //pin mode setting
  pinMode(flex0Pin, INPUT);
  pinMode(flex1Pin, INPUT);
  pinMode(flex2Pin, INPUT);
  pinMode(flex3Pin, INPUT);
  pinMode(flex4Pin, INPUT);
  //LPF value Initialize
  filteredValue[0] = analogRead(flex0Pin);
  filteredValue[1] = analogRead(flex1Pin);
  filteredValue[2] = analogRead(flex2Pin);
  filteredValue[3] = analogRead(flex3Pin);
  filteredValue[4] = analogRead(flex4Pin);
}

void FilterDeg(float default_alpha){
  alpha = default_alpha;
}

void InitServo(){
  int i =0;
    
  servoArr[0].attach(servo0Pin);
  servoArr[1].attach(servo1Pin);
  servoArr[2].attach(servo2Pin);
  servoArr[3].attach(servo3Pin);
  servoArr[4].attach(servo4Pin);

  for (i = 0; i < 5; i++) {
    servoArr[i].write(0);
  } 
}

void InitVibe(){
  
  pinMode(vibePin,OUTPUT);
  vibeState = false;
  vibeCount = 0;
  
}

void InitMPU(){
    
  Wire.begin();
  Wire.setClock(400000);

  Serial.begin(115200);

  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788);

  if (devStatus == 0) {
  Serial.println(F("Enabling DMP..."));
  mpu.setDMPEnabled(true);

  Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
  mpuIntStatus = mpu.getIntStatus();

  Serial.println(F("DMP ready! Waiting for first interrupt..."));
  dmpReady = true;

  packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
  Serial.print(F("DMP Initialization failed (code "));
  Serial.print(devStatus);
  Serial.println(F(")"));
  }

}

//Get Sensor Value
void GetFlexData(){
  flexValueArr[0] = analogRead(flex0Pin);
  flexValueArr[1] = analogRead(flex1Pin);
  flexValueArr[2] = analogRead(flex2Pin); 
  flexValueArr[3] = analogRead(flex3Pin); 
  flexValueArr[4] = analogRead(flex4Pin); 

  FiltFlexData();
}


void FiltFlexData(){
  int i = 0;
  for (i = 0; i < 5; i++) {
    filteredValue[i] = filteredValue[i] * (1 - alpha) + flexValueArr[i] * alpha;
    delay(10);

    // (50~110 degree)  // max min 
    if (filteredValue[i] <= flexMin[i]) filteredValue[i] = flexMin[i];
    else if (filteredValue[i] >= flexMax[i]) filteredValue[i] = flexMax[i];
    angleValue[i] = map((int)filteredValue[i], flexMin[i], flexMax[i], 50, 180);
    angleValue[i] *= -1;
    angleValue[i] += 230;
  }
  
}


void getYPR() {
 if (!dmpReady) return;

 while (!mpuInterrupt && fifoCount < packetSize) {
 }

 mpuInterrupt = false;
 mpuIntStatus = mpu.getIntStatus();

 fifoCount = mpu.getFIFOCount();

 if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
 mpu.resetFIFO();
 Serial.println(F("FIFO overflow!"));

 } else if (mpuIntStatus & 0x02) {

 while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

 mpu.getFIFOBytes(fifoBuffer, packetSize);
 
 fifoCount -= packetSize;

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

//Control Servo And Vibe Motor
void RotateServo(char * control_data){
  int i=0;

  for(i=0;i<5;i++){
    if (control_data[i] == '0') {
      servoArr[i].write(0);
    }
    else if (control_data[i] == '1') {
      servoArr[i].write(60);
    }
    else if (control_data[i] == '2') {
      servoArr[i].write(120);
    }
    else if (control_data[i] == '3') {
      servoArr[i].write(150);
    }
  }
  
}

void MakeVibe(){
  if(vibeState){
    analogWrite(vibePin,0);
    vibeCount += 1;
  }
  else{
    analogWrite(vibePin,255);
  }

  if(vibeCount > vibeDuration){
    vibeCount = 0;
    vibeState = false;
  }
}

void TurnVibeOn(){
   if(!vibeState){
    vibeState = true;
  }
}

//------------------------------Main----------------------------


void setup() 
{
  InitFlex();
  FilterDeg(0.5);
  InitServo();
  InitVibe();
  InitMPU();
} 

void loop() 
{
  test.ReceiveData();

  if(test.newData == true){
    // when recieve program start message, end message, vibration module message
    Serial.print("recvData : ");
    Serial.println(test.recvData);
    Serial.print("recvLen : ");
    Serial.println(test.recvLen);

    if(test.recvLen == 1){

      //when message is start message
      if(test.recvData[0] == '1'){
        SendButton = true;
        test.ClearArr();
      }
      //when message is end message
      else if(test.recvData[0] == '3'){
        SendButton = false;
        test.ClearArr();
      }
      else if(test.recvData[0] == '2'){
        TurnVibeOn();
        test.ClearArr();
      }

    }
    //when servo motor control data, vibe motor control data
    else if(test.recvLen == 5){
      Serial.println("rotate servo");
      RotateServo(test.recvData);
      test.ClearArr();
    }

    test.newData = false;
  }

  if(SendButton){
    Serial.println("send data");
    GetFlexData();
    getYPR();
    // testCode -> delete When Run this code
    angleValue[0] = 50;
    angleValue[1] = 50;
    angleValue[2] = 50;
    angleValue[3] = 50;
    angleValue[4] = 50;
    ypr[0] = 3.2;
    ypr[1] = 3.2;
    ypr[2] = 3.2;
    
    test.SendData(angleValue,ypr);
  }

  MakeVibe();

  /*
  //플로터 출력을 위한 임시 코드
  int data0 = *(flexData+0);
  int data1 = *(flexData+1);
  int data2 = *(flexData+2);
  int data3 = *(flexData+3);
  int data4 = *(flexData+4);
  Serial.print(data0);
  Serial.print(' '); 
  Serial.print(data1); 
  Serial.print(' ');
  Serial.print(data2); 
  Serial.print(' ');
  Serial.print(data3); 
  Serial.print(' ');
  Serial.println(data4); 

  int i=0;
  for(i=0;i<5;i++){
  mySerial.write(*(flexData+i));
  }
  */
  delay(5);
}
