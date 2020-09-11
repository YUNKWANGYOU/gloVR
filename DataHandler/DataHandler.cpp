
// include this library's description file
#include "DataHandler.h"
#include "Arduino.h"
// include description files for other libraries used (if any)
#include "SoftwareSerial.h"


DataHandler::DataHandler(uint8_t rxPin, uint8_t txPin) : mySerial(rxPin, txPin){
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	mySerial.begin(9600);
}
void DataHandler::InitFlex() {

	pinMode(Flex1Pin, INPUT);
	pinMode(Flex2Pin, INPUT);
//	pinMode(Flex3Pin, INPUT);
//	pinMode(Flex4Pin, INPUT);
//	pinMode(Flex5Pin, INPUT);

	filteredvalue[0] = analogRead(Flex1Pin);
//	filteredvalue[1] = analogRead(Flex2Pin);
//	filteredvalue[2] = analogRead(Flex3Pin);
//	filteredvalue[3] = analogRead(Flex4Pin);
//	filteredvalue[4] = analogRead(Flex5Pin);
}
bool DataHandler::isready(int* ptr) {
	if (ptr == nullptr)
		return false;
	else
		return true;
}
void DataHandler::FilterDeg(float alpha) {
	this->alpha = alpha;
}

/*bool DataHandler::InitZyro() {

}

bool DataHandler::InitSurvo() {

	servoArr[0].attach(Survo1Pin);
	servoArr[1].attach(Survo2Pin);
	servoArr[2].attach(Survo3Pin);
	servoArr[3].attach(Survo4Pin);
	servoArr[4].attach(Survo5Pin);

	for (int i = 0; i < 5; i++) {
		servoArr[i].write(0);
	}

	return 1;
}*/


//데이터 송신시 사용하는 메소드
uint8_t*  DataHandler::getFlexData() {
	// 플렉스 센서의 데이터를 받아 필터 적용 후 int형 배열(angle값)을 반환.
	flexValueArr[0] = analogRead(Flex1Pin);
	flexValueArr[1] = analogRead(Flex2Pin);
	flexValueArr[2] = analogRead(Flex3Pin);
	flexValueArr[3] = analogRead(Flex4Pin);
	flexValueArr[4] = analogRead(Flex5Pin);
	
	return filtFlexData(flexValueArr)
}

uint8_t* DataHandler::filtFlexData(uint16_t* dataArr) {
	// 플렉스 센서의 데이터를 배열로 전달받아 low pass filter를 적용하고 50 ~ 180의 범위로 변환.
	// 변환한 값을 uint8_t 배열로 반환. 반환하는 배열의 길이는 5
	uint8_t filteredvalue[5];
	uint8_t angledata[5]; //각도 변환 값
	for (int i = 0; i < 5; i++) {
		filteredvalue[i] = filteredvalue[i] * (1 - alpha) + dataArr[i] * alpha; //필터 계산식
		delay(10);

		//범위 지정 (50~110 degree)  //아마 범위가 다 달라서 max값 min값 측정한뒤에 범위도 배열로 정해서 해줘야함
		if (filteredvalue[i] <= 380) filteredvalue[i] = 380;
		else if (filteredvalue[i] >= 620) filteredvalue[i] = 620;
		angledata[i] = map((int)filteredvalue[i], 380, 620, 50, 180); //각도로 변환
		angledata[i] *= -1;
		angledata[i] += 230;
	}

	return angledata;
}
/*
int* DataHandler::getZyroData() {
	// 자이로 센서의 데이터를 받아 배열로 반환.

}

uint8_t* DataHandler::filtZyroData(int* dataArr) {
	// 자이로 센서의 데이터를 배열로 받아 00필터를 적용하고 00 ~ 000의 범위로 변환.
	// 반환한 값을 uint8_t 배열로 변환. 반환하는 배열의 길이는 5
}

bool DataHandler::setSendData(uint8_t* flexDataArr, uint8_t* zyroDataArr) {
	// 필터가 적용된 flexDataArr과 zyroDataArr을 전달받아 
	// arduinoToUnity배열에 대입.
	// flexDataArr의 길이는 5, zyroDataArr의 길이는 6

	int i = 0;
	for (i = 0; i < 5; i++) {
		arduinoToUnityDataArray[i + 1] = flexDataArr[i];
	}

	for (i = 0; i < 6; i++) {
		arduinoToUnityDataArray[i + 5] = zyroDataArr[i];
	}
}

char DataHandler::checkAllSendData() {
	// 보내기 전 arduinoToUnityDataArray배열의 데이터 검사.
}

bool DataHandler::sendData() {
	// arduinoToUnityDataArray를 블루투스 모듈을 통해서 전송.
	int i = 0;

	for (i = 0; i < 13; i++) {
		mySerial.write(arduinoToUnityDataArray[i]);
	}

	return 1;
}

//데이터 수신시 사용하는 메소드
int * DataHandler::ReceiveData() {
	// 블루투스 모듈을 통해서 데이터를 받아 unityToArduinoDataArray 배열에 저장.

}

bool DataHandler::RotateSurvo() {
	// unityToArduinoDataArray배열의 데이터에 따라 서보모터 각도를 회전.
}*/

