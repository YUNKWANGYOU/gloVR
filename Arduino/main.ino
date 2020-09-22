#include <DataHandler.h>

DataHandler d1;

void setup(){
  d1.Init();
}

void update(){
  //start

  if(d1.ReceiveData()){
    // when recieve program start message, end message, vibration module message
    if(d1.unityToArduinoDataArray[2] == 'e'){
      // when message is start message
      if(d1.unityToArduinoDataArray[1] == '1'){

      }

      // when message is vibration message
      if(d1.unityToArduinoDataArray[1] == '2'){

      }

      // when message is end message
      if(d1.unityToArduinoDataArray[1] == '3'){

      }
    }

    //when recieve servo motor controll message
    if(d1.unityToArduinoDataArray[6] == 'e'){
      d1.RotateServo();
    }
  }

}