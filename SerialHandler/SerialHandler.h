
// ensure this library description is only included once
#ifndef SerialHandler_h
#define SerialHandler_h

// include types & constants of Wiring core API
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "String.h"

#define receiveDataArrayLen 7

// library interface description
class SerialHandler
{
public:
	SerialHandler(uint8_t BluetoothRxPin, uint8_t BluetoothTxPin);
	
	//Arduino to Unity  setting
	char recvData[receiveDataArrayLen];
	boolean recvInProgress = false;
	byte ndx = 0;
	byte recvLen = 0;
	char startMarker = 's';
	char endMarker = 'e';
	char rc;
	boolean newData = false;

	SoftwareSerial mySerial;
	
	//Send and Receive Data
	void SendData(uint8_t * flexData, float * ypr);
	void ReceiveData();
    void ClearArr();
};

#endif

