#include <DataHandler.h>

bool DataHandler::InitFlex(){
    pinMode(Flex1Pin,INPUT);
    pinMode(Flex2Pin,INPUT);
    pinMode(Flex3Pin,INPUT);
    pinMode(Flex4Pin,INPUT);
    pinMode(Flex5Pin,INPUT);

    return 1;
}

bool DataHandler::InitZyro(){

}

bool DataHandler::InitSurvo(){
    
    servoArr[0].attach(Survo1Pin);
    servoArr[1].attach(Survo2Pin);
    servoArr[2].attach(Survo3Pin);
    servoArr[3].attach(Survo4Pin);
    servoArr[4].attach(Survo5Pin);

    for(int i=0;i<5;i++){
        servoArr[i].write(0)
    }

    return 1;
}

bool DataHandler::InitBluetooth(){

  mySerial.begin(9600);
  return 1;

}

//데이터 송신시 사용하는 메소드
char DataHandler::getFlexData(){
    // 플렉스 센서의 데이터를 받아 객체의 arduinoToUnityDataArray배열에 추가

}

char DataHandler::getZyroData(){
    // 자이로 센서의 데이터를 받아 객체의 arduinoToUnityDataArray배열에 추가

}

char DataHandler::setAllSendData(int * intArr){
    
}

bool DataHandler::sendData(){

}

//데이터 수신시 사용하는 메소드
int * DataHandler::ReceiveData(){

}

bool DataHandler::RotateSurvo(){

}