
// ensure this library description is only included once
#ifndef DataHandler_h
#define DataHandler_h

// include types & constants of Wiring core API
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Servo.h"


// 초기화하기
	//FlexSensor pin number
	#define flex0Pin A0
	#define flex1Pin A1
	#define flex2Pin A2
	#define flex3Pin A3
	#define flex4Pin A4

	//ZyroSensor pin number
	#define INTERRUPT_PIN 12

	//Survo motor pin number
	#define servo0Pin 5
	#define servo1Pin 6
	#define servo2Pin 9
	#define servo3Pin 10
	#define servo4Pin 11
	//define data len
	#define sendDataArrayLen 13
	#define receiveDataArrayLen 7

// library interface description
class DataHandler
{
public:
	DataHandler(uint8_t BluetoothRxPin, uint8_t BluetoothTxPin); //블루투스 핀 지정하고 클래스 객체 생성
	//서보모터 단계 배열
	Servo servoArr[5];
	//flex 데이터 생성시 필요한 배열, 변수
	float alpha;                                       // 필터링 민감도
	uint16_t flexValueArr[5];
	uint16_t filteredValue[5];
	uint8_t angleValue[5];
	uint16_t flexMax[5] = { 820, 690, 890, 770, 880 };
	uint16_t flexMin[5] ={ 710, 500, 800, 580, 780 };

	//zyro 데이터 생성시 필요한 배열, 변수
	bool dmpReady = false;
	uint8_t mpuIntStatus;
	uint8_t devStatus;
	uint16_t packetSize;
	uint16_t fifoCount;
	uint8_t fifoBuffer[64];
	volatile bool mpuInterrupt = false;

	//통신을 위한 버퍼, 블루투스 객체 선언
	char arduinoToUnityDataArray[sendDataArrayLen];
	char unityToArduinoDataArray[receiveDataArrayLen];
	SoftwareSerial mySerial; // 블루투스 객체
	void (*fcnPtr)();
	void dmpDataReady();


	//서보 모터 및 각종 센서, 모듈 초기값 지정
	void InitFlex();
	void FilterDeg(float alpha);
	void  GetFlexRange();
	void InitZyro();
	void InitServo();
	bool IsReady(int* ptr); 

	//Sensor Value
	uint8_t* GetFlexData(); //uint8_t anglevalue[5] 반환함
	void FiltFlexData();
	void GetZyroData();
	void FiltZyroData(int* dataArr);
	
	//Send Data
	void SetSendData(uint8_t* flexDataArr, uint8_t* zyroDataArr);
	void CheckAllSendData();
	void SendData();

	//Receive Data
	uint8_t* ReceiveData();
	void RotateServo();
};

#endif

