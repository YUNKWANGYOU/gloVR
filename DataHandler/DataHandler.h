
// ensure this library description is only included once
#ifndef DataHandler_h
#define DataHandler_h

// include types & constants of Wiring core API
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Servo.h"
#include "String.h"


// PIN setting
	//FlexSensor pin number
	#define flex0Pin A0
	#define flex1Pin A1
	#define flex2Pin A2
	#define flex3Pin A3
	#define flex4Pin A6

	//Servo motor pin number
	#define servo0Pin 3
	#define servo1Pin 4
	#define servo2Pin 5
	#define servo3Pin 6
	#define servo4Pin 7

	//define data len
	#define receiveDataArrayLen 7

// library interface description
class DataHandler
{
public:
	DataHandler(uint8_t BluetoothRxPin, uint8_t BluetoothTxPin);
	Servo servoArr[5];
	float alpha;                                      
	uint16_t filteredValue[5];
	uint16_t flexMax[5] = { 820, 690, 890, 770, 880 };
	uint16_t flexMin[5] ={ 710, 500, 800, 580, 780 };
	
	//Arduino to Unity  setting
	char unityToArduinoDataArray[receiveDataArrayLen];
	SoftwareSerial mySerial;

	//Flex setting
	void InitFlex();
	void FilterDeg(float alpha);
	void GetFlexRange(); //주석처리되어있음
	void InitServo();
	bool IsReady(int* ptr); //? ptr?

	//Sensor Value
	uint8_t* GetFlexData(); //uint8_t anglevalue[5]
	uint8_t* FiltFlexData(uint16_t* flexValueArr);
	
	//Send Data
	void SendData(uint8_t * flexData, char * ypr);

	//Receive Data
	uint8_t* ReceiveData();
	void RotateServo();
};

#endif

