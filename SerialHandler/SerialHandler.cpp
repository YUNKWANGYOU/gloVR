
// include this library's description file
#include "SerialHandler.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "String.h"


SerialHandler::SerialHandler(uint8_t rxPin, uint8_t txPin) : mySerial(rxPin, txPin){
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	mySerial.begin(115200);
}

void SerialHandler::SendData(uint8_t * flexData, float * ypr) {

	char pChrBuffer[6];
  	int i=0;

	mySerial.write(200);

	for(i=0;i<5;i++){
	mySerial.write(flexData[i]);
	}
  
	dtostrf(ypr[0] , 5, 2, pChrBuffer);  // 5 : width, 2 : precision
	mySerial.write(pChrBuffer);
	mySerial.write('\n');
	dtostrf(ypr[1] , 5, 2, pChrBuffer);  // 5 : width, 2 : precision
	mySerial.write(pChrBuffer);
	mySerial.write('\n');
	dtostrf(ypr[2] , 5, 2, pChrBuffer);  // 5 : width, 2 : precision
	mySerial.write(pChrBuffer);
	mySerial.write('\n');
	
	mySerial.write(201);
}


void SerialHandler::ReceiveData() {
	// recvData
	int i=0;

	while(mySerial.available() > 0 && newData == false){
		rc = mySerial.read();
		Serial.println(rc);

		if(recvInProgress == true){
			if(rc != endMarker){
				recvData[ndx] = rc;
				ndx++;
				if(ndx >= receiveDataArrayLen - 1){
					ndx = receiveDataArrayLen - 2;
				}
			}
			else{
				if(ndx <= receiveDataArrayLen - 2){
					recvData[ndx + 1] = '\0';
					recvInProgress = false;
					recvLen = ndx;
					ndx = 0;
					newData = true;
				}
				else{
					ClearArr();
					recvInProgress = false;
					ndx = 0;
					newData = false;
				}
			}
			Serial.println(recvData);
		}

		else if(rc == startMarker){
			Serial.println("okay go");
			recvInProgress = true;
		}
	}
}


void SerialHandler::ClearArr(){
	int i=0;
	for(i=0;i<7;i++){
		recvData[i] = '\0';
	}
}