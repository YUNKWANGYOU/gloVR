
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

/*bool DataHandler::InitZyro() {

}

bool DataHandler::InitSurvo() {

	servoArr[0].attach(Survo1Pin);
	servoArr[1].attach(Survo2Pin);
	servoArr[2].attach(Survo3Pin);
	servoArr[3].attach(Survo4Pin);
	servoArr[4].attach(Survo5Pin);

	for (int i = 0; i < 5; i++) {
		servoArr[i].write(0);
	}

	return 1;
}*/


//������ �۽Ž� ����ϴ� �޼ҵ�
uint8_t*  DataHandler::GetFlexData() {
	// �÷��� ������ �����͸� �޾� ���� ���� �� int�� �迭(angle��)�� ��ȯ.
	flexValueArr[0] = analogRead(flex0Pin); 
	flexValueArr[1] = analogRead(flex1Pin);
	flexValueArr[2] = analogRead(flex2Pin); //��������
	flexValueArr[3] = analogRead(flex3Pin); //��������
	flexValueArr[4] = analogRead(flex4Pin); //��������

	FiltFlexData(); // ���͸�
	return angleValue; // ������ �迭 �ּҰ� ��ȯ
}

void DataHandler::FiltFlexData() {
	// �÷��� ������ �����͸� �迭�� ���޹޾� low pass filter�� �����ϰ� 50 ~ 180�� ������ ��ȯ.
	// ��ȯ�� ���� uint8_t �迭�� ��ȯ. ��ȯ�ϴ� �迭�� ���̴� 5

	for (int i = 0; i < 5; i++) {
		filteredValue[i] = filteredValue[i] * (1 - alpha) + flexValueArr[i] * alpha; //���� ����
		delay(10);

		//���� ���� (50~110 degree)  //�Ƹ� ������ �� �޶� max�� min�� �����ѵڿ� ������ �迭�� ���ؼ� �������
		if (filteredValue[i] <= 500) filteredValue[i] = 500;
		else if (filteredValue[i] >= 800) filteredValue[i] = 800;
		angleValue[i] = map((int)filteredValue[i], 500, 800, 50, 180); //������ ��ȯ
		angleValue[i] *= -1; // 180 ~ 50 ������ 50 ~ 180 ���� ��ȯ����
		angleValue[i] += 230; 
	}
}
/*
int* DataHandler::getZyroData() {
	// ���̷� ������ �����͸� �޾� �迭�� ��ȯ.

}

uint8_t* DataHandler::filtZyroData(int* dataArr) {
	// ���̷� ������ �����͸� �迭�� �޾� 00���͸� �����ϰ� 00 ~ 000�� ������ ��ȯ.
	// ��ȯ�� ���� uint8_t �迭�� ��ȯ. ��ȯ�ϴ� �迭�� ���̴� 5
}

bool DataHandler::setSendData(uint8_t* flexDataArr, uint8_t* zyroDataArr) {
	// ���Ͱ� ����� flexDataArr�� zyroDataArr�� ���޹޾� 
	// arduinoToUnity�迭�� ����.
	// flexDataArr�� ���̴� 5, zyroDataArr�� ���̴� 6

	int i = 0;
	for (i = 0; i < 5; i++) {
		arduinoToUnityDataArray[i + 1] = flexDataArr[i];
	}

	for (i = 0; i < 6; i++) {
		arduinoToUnityDataArray[i + 5] = zyroDataArr[i];
	}
}

char DataHandler::checkAllSendData() {
	// ������ �� arduinoToUnityDataArray�迭�� ������ �˻�.
}

bool DataHandler::sendData() {
	// arduinoToUnityDataArray�� ������� ����� ���ؼ� ����.
	int i = 0;

	for (i = 0; i < 13; i++) {
		mySerial.write(arduinoToUnityDataArray[i]);
	}

	return 1;
}

//������ ���Ž� ����ϴ� �޼ҵ�
int * DataHandler::ReceiveData() {
	// ������� ����� ���ؼ� �����͸� �޾� unityToArduinoDataArray �迭�� ����.

}

bool DataHandler::RotateSurvo() {
	// unityToArduinoDataArray�迭�� �����Ϳ� ���� �������� ������ ȸ��.
}*/

