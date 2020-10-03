#include "TestHandler.h"
#include <SoftwareSerial.h>

TestHandler test(8,9);
uint8_t *flexData;

bool SendButton = false;
bool newData = false;

float ypr[3] = {-3.2,-3.2,-3.2};

void InitMPU(){
    Serial.println("Init MPU");
}


void getYPR() {
    int i=0;
    for(i=0;i<3;i++){
        ypr[i] = ypr[i] + 0.01;
        if(ypr[i] > 3.2){
            ypr[i] = -3.2;
        }
    }
}

void setup() 
{
  Serial.begin(9600);
  test.InitFlex();
  test.FilterDeg(0.5);
  test.InitServo();
  test.InitVibe();
  
  InitMPU();
} 

void loop() 
{
  newData = test.ReceiveData();
  delay(5);

  if(newData){
    delay(1000);
    // when recieve program start message, end message, vibration module message
    if(test.unityToArduinoDataArray[2] == 'e'){

      //when message is start message
      if(test.unityToArduinoDataArray[1] == '1'){
        Serial.println("receive start_string");
        Serial.println(test.unityToArduinoDataArray);
        SendButton = true;
        test.ClearArr();
      }
      //when message is end message
      else if(test.unityToArduinoDataArray[1] == '3'){
        Serial.println(test.unityToArduinoDataArray);
        Serial.println("receive end_string");
        SendButton = false;
        test.ClearArr();
      }

    }
    //when servo motor control data, vibe motor control data
    else if(test.unityToArduinoDataArray[6] == 'e'){
      Serial.println("rotate servo");
      test.RotateServo();
      test.TurnVibeOn();
      test.ClearArr();
    }


  }

  if(SendButton){
    Serial.println("send data");
    flexData = test.GetFlexData();
    getYPR();
    test.SendData(flexData,ypr);
  }

  test.MakeVibe();

  delay(5);
 
}
