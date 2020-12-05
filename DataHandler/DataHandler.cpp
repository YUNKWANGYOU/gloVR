
// include this library's description file
#include "DataHandler.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "String.h"
#include "Servo.h"


DataHandler::DataHandler(uint8_t rxPin, uint8_t txPin) : mySerial(rxPin, txPin){
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


void DataHandler::InitServo() {
	
	servoArr[0].attach(servo0Pin); //엄지
	servoArr[1].attach(servo1Pin); //검지
	servoArr[2].attach(servo2Pin); 
	servoArr[3].attach(servo3Pin);
	servoArr[4].attach(servo4Pin); //새끼

	// for (int i = 0; i < 5; i++) {
	// 	servoArr[i].write(0);
	// }

	servoArr[0].write(150);
	servoArr[1].write(150);
	servoArr[2].write(150);
	servoArr[3].write(10);
	servoArr[4].write(10);

}

void DataHandler::InitVibe(){
	pinMode(vibePin,OUTPUT);
	vibeState = false;
	vibeNum = 0;
	analogWrite(vibePin,0);
}

uint8_t* DataHandler::GetFlexData() {
	
	flexValueArr[0] = analogRead(flex0Pin);
	flexValueArr[1] = analogRead(flex1Pin);
	flexValueArr[2] = analogRead(flex2Pin); 
	flexValueArr[3] = analogRead(flex3Pin); 
	flexValueArr[4] = analogRead(flex4Pin); 

	// Serial.print("flex Value : ");
	// Serial.print(flexValueArr[0]);
	// Serial.print(flexValueArr[1]);
	// Serial.print(flexValueArr[2]);
	// Serial.print(flexValueArr[3]);
	// Serial.print(flexValueArr[4]);
	// Serial.println();


	FiltFlexData();

	return angleValue;
	// return flexValueArr;
}

void DataHandler::FiltFlexData() {
	// low pass filter 50 ~ 180
	// uint8_t 5

	for (int i = 0; i < 5; i++) {
		filteredValue[i] = filteredValue[i] * (1 - alpha) + flexValueArr[i] * alpha; //���� ����
		delay(10);

		// (50~110 degree)  // max min 
		if (filteredValue[i] <= flexMin[i]) filteredValue[i] = flexMin[i];
		else if (filteredValue[i] >= flexMax[i]) filteredValue[i] = flexMax[i];
		angleValue[i] = map((int)filteredValue[i], flexMin[i], flexMax[i], 50, 180); //������ ��ȯ
		angleValue[i] *= -1; // 180 ~ 50 50 ~ 180 
		angleValue[i] += 230;
	}

}

void DataHandler::SendData(uint8_t * flexData, uint8_t * teapotPacket) {
	// arduinoToUnityDataArray�� �������� ����� ���ؼ� ����.

  	int i=0;

	mySerial.write(200);

	for(i=0;i<5;i++){
	mySerial.write(flexData[i]);
	}

	mySerial.write(teapotPacket, 14);

}


void DataHandler::ReceiveData() {
	// unityToArduinoDataArray
	int i=0;

	while(mySerial.available() > 0 && newData == false){
		rc = mySerial.read();
		Serial.println(rc);

		if(recvInProgress == true){
			if(rc != endMarker){
				unityToArduinoDataArray[ndx] = rc;
				ndx++;
				if(ndx >= receiveDataArrayLen - 1){
					ndx = receiveDataArrayLen - 2;
				}
			}
			else{
				if(ndx <= receiveDataArrayLen - 2){
					unityToArduinoDataArray[ndx + 1] = '\0';
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
			Serial.println(unityToArduinoDataArray);
		}

		else if(rc == startMarker){
			Serial.println("okay go");
			recvInProgress = true;
		}
	}
}

void DataHandler::RotateServo() {
	// unityToArduinoDataArray

	int i=0;

	// for(i=0;i<5;i++){
	// 	if (unityToArduinoDataArray[i] == '0') {
	// 	servoArr[i].write(0);
	// 	}
	// 	else if (unityToArduinoDataArray[i] == '1') {
	// 		servoArr[i].write(60);
	// 	}
	// 	else if (unityToArduinoDataArray[i] == '2') {
	// 		servoArr[i].write(120);
	// 	}
	// 	else if (unityToArduinoDataArray[i] == '3') {
	// 		servoArr[i].write(150);
	// 	}
	// }

	//새끼
	if (unityToArduinoDataArray[0] == '0') {
		servoArr[4].write(10);
	}
	else if (unityToArduinoDataArray[0] == '1') {
		servoArr[4].write(60);
	}
	else if (unityToArduinoDataArray[0] == '2') {
		servoArr[4].write(120);
	}
	else if (unityToArduinoDataArray[0] == '3') {
		servoArr[4].write(150);
	}

	//약지
	if (unityToArduinoDataArray[1] == '0') {
		servoArr[3].write(10);
	}
	else if (unityToArduinoDataArray[1] == '1') {
		servoArr[3].write(60);
	}
	else if (unityToArduinoDataArray[1] == '2') {
		servoArr[3].write(120);
	}
	else if (unityToArduinoDataArray[1] == '3') {
		servoArr[3].write(150);
	}

	//중지
	if (unityToArduinoDataArray[2] == '0') {
		servoArr[2].write(150);
	}
	else if (unityToArduinoDataArray[2] == '1') {
		servoArr[2].write(120);
	}
	else if (unityToArduinoDataArray[2] == '2') {
		servoArr[2].write(60);
	}
	else if (unityToArduinoDataArray[2] == '3') {
		servoArr[2].write(0);
	}

	//검지
	if (unityToArduinoDataArray[3] == '0') {
		servoArr[1].write(150);
	}
	else if (unityToArduinoDataArray[3] == '1') {
		servoArr[1].write(120);
	}
	else if (unityToArduinoDataArray[3] == '2') {
		servoArr[1].write(60);
	}
	else if (unityToArduinoDataArray[3] == '3') {
		servoArr[1].write(0);
	}

	//엄지
	if (unityToArduinoDataArray[4] == '0') {
		servoArr[0].write(150);
	}
	else if (unityToArduinoDataArray[4] == '1') {
		servoArr[0].write(120);
	}
	else if (unityToArduinoDataArray[4] == '2') {
		servoArr[0].write(60);
	}
	else if (unityToArduinoDataArray[4] == '3') {
		servoArr[0].write(0);
	}

}

void DataHandler::MakeVibe(){
	if(vibeState){
		analogWrite(vibePin,255);
		vibeNum += 1;
	}
	else{
		analogWrite(vibePin,0);
	}

	if(vibeNum > vibeDuration){
		vibeNum = 0;
		vibeState = false;
	}
}

void DataHandler::TurnVibeOn(){
	if(!vibeState){
		vibeState = true;
	}
}

void DataHandler::ClearArr(){
	int i=0;
	for(i=0;i<7;i++){
		unityToArduinoDataArray[i] = '\0';
	}
}

void DataHandler::DetachServo(){

		servoArr[0].detach();
		servoArr[1].detach();
		servoArr[2].detach();
		servoArr[3].detach();
		servoArr[4].detach();
}

