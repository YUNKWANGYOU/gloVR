
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
bool DataHandler::IsReady(int* ptr) {
	if (ptr == nullptr)
		return false;
	else
		return true;
}
void DataHandler::FilterDeg(float alpha) {
	this->alpha = alpha;
}

void DataHandler::InitZyro() {

}

void DataHandler::InitServo() {

	servoArr[0].attach(servo0Pin);
	servoArr[1].attach(servo1Pin);
	servoArr[2].attach(servo2Pin);
	servoArr[3].attach(servo3Pin);
	servoArr[4].attach(servo4Pin);

	for (int i = 0; i < 5; i++) {
		servoArr[i].write(0);
	}

	return 1;
}


//데이터 송신시 사용하는 메소드
uint8_t*  DataHandler::GetFlexData() {
	// 플렉스 센서의 데이터를 받아 필터 적용 후 int형 배열(angle값)을 반환.
	flexValueArr[0] = analogRead(flex0Pin); 
	flexValueArr[1] = analogRead(flex1Pin);
	flexValueArr[2] = analogRead(flex2Pin); //가변저항
	flexValueArr[3] = analogRead(flex3Pin); //가변저항
	flexValueArr[4] = analogRead(flex4Pin); //가변저항

	FiltFlexData(); // 필터링
	return angleValue; // 각도값 배열 주소값 반환
}

void DataHandler::FiltFlexData() {
	// 플렉스 센서의 데이터를 배열로 전달받아 low pass filter를 적용하고 50 ~ 180의 범위로 변환.
	// 변환한 값을 uint8_t 배열로 반환. 반환하는 배열의 길이는 5

	for (int i = 0; i < 5; i++) {
		filteredValue[i] = filteredValue[i] * (1 - alpha) + flexValueArr[i] * alpha; //필터 계산식
		delay(10);

		//범위 지정 (50~110 degree)  //아마 범위가 다 달라서 max값 min값 측정한뒤에 범위도 배열로 정해서 해줘야함
		if (filteredValue[i] <= flexMin[i]) filteredValue[i] = flexMin[i];
		else if (filteredValue[i] >= flexMax[i]) filteredValue[i] = flexMax[i];
		angleValue[i] = map((int)filteredValue[i], flexMin[i], flexMax[i], 50, 180); //각도로 변환
		angleValue[i] *= -1; // 180 ~ 50 범위를 50 ~ 180 으로 변환해줌
		angleValue[i] += 230; 
	}
}

uint8_t* DataHandler::GetZyroData() {
	// 자이로 센서의 데이터를 받아 배열로 반환.

}

void DataHandler::FiltZyroData(int* dataArr) {
	// 자이로 센서의 데이터를 배열로 받아 00필터를 적용하고 00 ~ 000의 범위로 변환.
	// 반환한 값을 uint8_t 배열로 변환. 반환하는 배열의 길이는 5
}

void  DataHandler::SetSendData(uint8_t* flexDataArr, uint8_t* zyroDataArr) {
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

void DataHandler:: CheckAllSendData() {
	// 보내기 전 arduinoToUnityDataArray배열의 데이터 검사.
}

void DataHandler::SendData() {
	// arduinoToUnityDataArray를 블루투스 모듈을 통해서 전송.
	int i = 0;

	for (i = 0; i < 13; i++) {
		mySerial.write(arduinoToUnityDataArray[i]);
	}
}

//데이터 수신시 사용하는 메소드
uint8_t* DataHandler::ReceiveData() {
	// 블루투스 모듈을 통해서 데이터를 받아 unityToArduinoDataArray 배열에 저장.

}

void DataHandler::RotateServo() {
	// unityToArduinoDataArray배열의 데이터에 따라 서보모터 각도를 회전.
}

