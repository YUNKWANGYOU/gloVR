#include <DataHandler.h>

bool DataHandler::InitFlex(){
    pinMode(Flex1Pin,INPUT);
    pinMode(Flex2Pin,INPUT);
    pinMode(Flex3Pin,INPUT);
    pinMode(Flex4Pin,INPUT);
    pinMode(Flex5Pin,INPUT);
}

bool DataHandler::InitZyro(){

}

bool DataHandler::InitSurvo(){


}

bool DataHandler::InitBluetooth(){

}

//데이터 송신시 사용하는 메소드
char DataHandler::getFlexData(int * intArr){

}

char DataHandler::getZyroData(int * intArr){

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