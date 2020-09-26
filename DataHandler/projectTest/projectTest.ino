#include "DataHandler.h"
#include <SoftwareSerial.h>

#define RxPin 6
#define TxPin 7

DataHandler test(2,3);
uint8_t *flexData;
uint8_t *zyroData;
SoftwareSerial mySerial(RxPin,TxPin);

/*void dmpDataReady() {
    mpuInterrupt = true;
}*/


void setup() 
{ 
  Serial.begin(9600);
  mySerial.begin(9600);
  //pinMode(INTERRUPT_PIN, INPUT);
  //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
  test.InitFlex();
 // test.GetFlexRange();
  test.InitZyro();
  test.InitServo();
  test.FilterDeg(0.7);
} 

void loop() 
{ 
 //센서값 전송
 flexData = test.GetFlexData();
// test.GetZyroData();
// test.SetSendData(flexData, zyroData);
// test.CheckAllSendData();
// test.SendData();

 //서보모터 값, 진동모터 값 송신
 if(test.ReceiveData()){
    // when recieve program start message, end message, vibration module message
    if(test.unityToArduinoDataArray[2] == 'e'){
      // when message is start message
      if(test.unityToArduinoDataArray[1] == '1'){

      }

      // when message is vibration message
      if(test.unityToArduinoDataArray[1] == '2'){

      }

      // when message is end message
      if(test.unityToArduinoDataArray[1] == '3'){

      }
    }

    //when recieve servo motor controll message
    if(test.unityToArduinoDataArray[6] == 'e'){
      test.RotateServo();
    }
  }





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

 delay(5);
 
}
