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
	DataHandler(uint8_t BluetoothRxPin, uint8_t BluetoothTxPin);
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
	float alpha;
	uint16_t flexValueArr[5];
	uint16_t filteredvalue[5];
	SoftwareSerial mySerial;
	void InitFlex();
	void FilterDeg(float alpha);
	void InitZyro();
	//bool InitSurvo();
	//bool InitBluetooth();
	bool isready(int* ptr); // 에러확인

	//데이터 송신시 사용하는 메소드
	uint8_t* getFlexData();
	uint8_t* filtFlexData(uint16_t* dataArr)

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

