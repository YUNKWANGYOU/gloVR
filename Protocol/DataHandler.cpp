#include <DataHandler.h>

bool DataHandler::InitFlexFilter(){
    
    flexValueArr[0] = analogRead(Flex1Pin);
    flexValueArr[1] = analogRead(Flex2Pin);
    flexValueArr[2] = analogRead(Flex3Pin);
    flexValueArr[3] = analogRead(Flex4Pin);
    flexValueArr[4] = analogRead(Flex5Pin);

    return 1;
}

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
        servoArr[i].write(0);
    }

    return 1;
}

bool DataHandler::InitBluetooth(){

  mySerial.begin(9600);
  return 1;

}

//데이터 송신시 사용하는 메소드
int* DataHandler::getFlexData(){
    // 플렉스 센서의 데이터를 받아 int형 배열을 반환.

    int readDataArr[5];
    readDataArr[0] = analogRead(Flex1Pin);
    readDataArr[1] = analogRead(Flex2Pin);
    readDataArr[2] = analogRead(Flex3Pin);
    readDataArr[3] = analogRead(Flex4Pin);
    readDataArr[4] = analogRead(Flex5Pin);

    return readDataArr
}

uint8_t* DataHandler::filtFlexData(int* dataArr){
    // 플렉스 센서의 데이터를 배열로 전달받아 low pass filter를 적용하고 50 ~ 180의 범위로 변환.
    // 변환한 값을 uint8_t 배열로 반환. 반환하는 배열의 길이는 5

    int i=0;
    uint8_t filteredDataArr[5];

    //low pass filter 적용.
    for(i=0;i<5;i++){
        flexValueArr[i] = flexValueArr[i] * (1 - alpha) + dataArr[i] * alpha;
        // filteredvalue = filteredvalue * (1 - alpha) + flexvalue * alpha; 
    }

    //map 함수로 데이터를 50 ~ 180의 범위로 변환.
    for(i=0;i<5;i++){
        //map()함수 사용.
    }

    return filterdDataArr[5];
}

int* DataHandler::getZyroData(){
    // 자이로 센서의 데이터를 받아 배열로 반환.

}

uint8_t* DataHandler::filtZyroData(int* dataArr){
    // 자이로 센서의 데이터를 배열로 받아 00필터를 적용하고 00 ~ 000의 범위로 변환.
    // 반환한 값을 uint8_t 배열로 변환. 반환하는 배열의 길이는 5
}

bool DataHandler::setSendData(uint8_t* flexDataArr, uint8_t* zyroDataArr){
    // 필터가 적용된 flexDataArr과 zyroDataArr을 전달받아 
    // arduinoToUnity배열에 대입.
    // flexDataArr의 길이는 5, zyroDataArr의 길이는 6

    int i=0;
    for(i=0;i<5;i++){
        arduinoToUnityDataArray[i+1] = flexDataArr[i];
    }

    for(i=0;i<6;i++){
        arduinoToUnityDataArray[i+5] = zyroDataArr[i];
    }

}

char DataHandler::checkAllSendData(){
    // 보내기 전 arduinoToUnityDataArray배열의 데이터 검사.
}

bool DataHandler::sendData(){
    // arduinoToUnityDataArray를 블루투스 모듈을 통해서 전송.
    int i=0;

    for(i=0;i<13;i++){
        mySerial.write(arduinoToUnityDataArray[i]);
    }
    
}

//데이터 수신시 사용하는 메소드
int * DataHandler::ReceiveData(){
    // 블루투스 모듈을 통해서 데이터를 받아 unityToArduinoDataArray 배열에 저장.

}

bool DataHandler::RotateSurvo(){
    // unityToArduinoDataArray배열의 데이터에 따라 서보모터 각도를 회전.
}

//--------------------------헬퍼함수----------------------

uint8_t filtFlexData(int data){


}