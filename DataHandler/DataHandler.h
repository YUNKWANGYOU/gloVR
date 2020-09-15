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
	DataHandler(uint8_t BluetoothRxPin, uint8_t BluetoothTxPin); //������� �� �����ϰ� Ŭ���� ��ü ����
	//arduino -> unity data
	// start record(0), flex Sensor(1 ~ 5), zyro Sensor(5 ~ 11), end record(12)
	//char arduinoToUnityDataArray[sendDataArrayLen];

	//unity -> arduino data
	// start record(0), param(1~5), end record(6)
	//char unityToArduinoDataArray[receiveDataArrayLen];

	//bluetooth module


	//servo motor array
	//Servo servoArr[5];

	// �ʱ�ȭ�ϱ�
	int flex0Pin = A0;
	int flex1Pin = A1;
	int flex2Pin = A2;
	int flex3Pin = A3;
	int flex4Pin = A4;
	float alpha; // ���͸� �ΰ��� �Ƶ��̳뿡�� ���� ����
	uint16_t flexValueArr[5];
	uint16_t filteredValue[5];
	uint8_t angleValue[5];
	SoftwareSerial mySerial; // ������� ��ü

	void InitFlex();
	void FilterDeg(float alpha);
	void InitZyro();
	//bool InitSurvo();
	//bool InitBluetooth();
	bool IsReady(int* ptr); // ����Ȯ�ο� ���� ��� x

	//������ �۽Ž� ����ϴ� �޼ҵ�
	uint8_t* GetFlexData(); //uint8_t anglevalue[5] ��ȯ��
	void FiltFlexData();

	/*int* getZyroData();  
	uint8_t* filtZyroData(int* dataArr);

	bool setSendData(uint8_t* flexDataArr, uint8_t* zyroDataArr);

	char checkAllSendData(int * intArr);

	bool sendData();

	//������ ���Ž� ����ϴ� �޼ҵ�
	int * ReceiveData();
	bool RotateSurvo();*/
};

#endif

