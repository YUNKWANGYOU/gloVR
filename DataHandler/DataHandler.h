
// ensure this library description is only included once
#ifndef DataHandler_h
#define DataHandler_h

// include types & constants of Wiring core API
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Servo.h"
#include "I2Cdev.h"
#include "Wire.h"
#include "String.h"
#include "MPU6050_6Axis_MotionApps20.h"


// PIN setting
	//FlexSensor pin number
	#define flex0Pin A0
	#define flex1Pin A1
	#define flex2Pin A2
	#define flex3Pin A3
	#define flex4Pin A6

	//GyroSensor pin number
	#define INTERRUPT_PIN 2
	#define SDAPIN A4
	#define SCLPIN A5

	//Servo motor pin number
	#define servo0Pin 3
	#define servo1Pin 4
	#define servo2Pin 5
	#define servo3Pin 6
	#define servo4Pin 7

	//define data len
	#define sendDataArrayLen 13
	#define receiveDataArrayLen 7

// library interface description
class DataHandler
{
public:
	DataHandler(uint8_t BluetoothRxPin, uint8_t BluetoothTxPin); 
	Servo servoArr[5]; 
	float alpha;                                      
	uint16_t flexValueArr[5];
	uint16_t filteredValue[5];
	uint8_t angleValue[5];
	uint16_t flexMax[5] = { 820, 690, 890, 770, 880 };
	uint16_t flexMin[5] ={ 710, 500, 800, 580, 780 };

	//Gyro setting
	bool dmpReady = false;
	uint8_t mpuIntStatus;
	uint8_t devStatus;
	uint16_t packetSize;
	uint16_t fifoCount;
	uint8_t fifoBuffer[64];
	volatile bool mpuInterrupt = false;

	MPU6050 mpu;

	//Yaw / Pitch / Roll
	Quaternion q;           // [w, x, y, z]         quaternion container
	VectorFloat gravity;    // [x, y, z]            gravity vector
	float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
	
	//Arduino to Unity  setting
	char arduinoToUnityDataArray[sendDataArrayLen];
	char unityToArduinoDataArray[receiveDataArrayLen];
	SoftwareSerial mySerial;
	void (*fcnPtr)(); //dmp intterupt check
	void dmpDataReady();


	//Flex setting
	void InitFlex();
	void FilterDeg(float alpha);
	void GetFlexRange(); //주석처리되어있음
	void InitZyro();
	void InitServo();
	bool IsReady(int* ptr); //? ptr?

	//Sensor Value
	uint8_t* GetFlexData(); //uint8_t anglevalue[5]
	uint8_t* FiltFlexData();

	uint8_t* GetZyroData();
	uint8_t* FiltZyroData();
	uint16_t floatToInt(float ypr)
	
	//Send Data
	void SetSendData(uint8_t* flexDataArr, uint8_t* zyroDataArr);
	void CheckAllSendData();
	void SendData();

	//Receive Data
	uint8_t* ReceiveData();
	void RotateServo();
};

#endif

