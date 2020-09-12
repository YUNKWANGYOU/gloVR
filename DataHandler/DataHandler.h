/*
  Test.h - Test library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// ensure this library description is only included once
#ifndef DataHandler_h
#define DataHandler_h

// include types & constants of Wiring core API
#include "Arduino.h"
#include "SoftwareSerial.h"



// library interface description
class DataHandler
{
public:
	DataHandler(uint8_t BluetoothRxPin, uint8_t BluetoothTxPin); //블루투스 핀 지정하고 클래스 객체 생성
	//arduino -> unity data
	// start record(0), flex Sensor(1 ~ 5), zyro Sensor(5 ~ 11), end record(12)
	//char arduinoToUnityDataArray[sendDataArrayLen];

	//unity -> arduino data
	// start record(0), param(1~5), end record(6)
	//char unityToArduinoDataArray[receiveDataArrayLen];

	//bluetooth module


	//servo motor array
	//Servo servoArr[5];

	// 초기화하기
	int Flex1Pin = A0;
	int Flex2Pin = A1;
	int Flex3Pin = A2;
	int Flex4Pin = A3;
	int Flex5Pin = A4;
	float alpha; // 필터링 민감도 아두이노에서 설정 가능
	uint16_t flexValueArr[5];
	uint16_t filteredvalue[5];
	uint8_t anglevalue[5];
	SoftwareSerial mySerial; // 블루투스 객체
	void InitFlex();
	void FilterDeg(float alpha);
	void InitZyro();
	//bool InitSurvo();
	//bool InitBluetooth();
	bool isready(int* ptr); // 에러확인용 아직 사용 x

	//데이터 송신시 사용하는 메소드
	uint8_t* getFlexData(); //uint8_t anglevalue[5] 반환함
	void filtFlexData();

	/*int* getZyroData();
	uint8_t* filtZyroData(int* dataArr);

	bool setSendData(uint8_t* flexDataArr, uint8_t* zyroDataArr);

	char checkAllSendData(int * intArr);

	bool sendData();

	//데이터 수신시 사용하는 메소드
	int * ReceiveData();
	bool RotateSurvo();*/
};

#endif

