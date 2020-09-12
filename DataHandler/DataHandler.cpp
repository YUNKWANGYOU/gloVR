
// include this library's description file
#include "DataHandler.h"
#include "Arduino.h"
// include description files for other libraries used (if any)
#include "SoftwareSerial.h"


DataHandler::DataHandler(uint8_t rxPin, uint8_t txPin) : mySerial(rxPin, txPin) {
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	mySerial.begin(9600);
}
void DataHandler::InitFlex() {

	pinMode(Flex1Pin, INPUT);
	pinMode(Flex2Pin, INPUT);
	//	pinMode(Flex3Pin, INPUT);
	//	pinMode(Flex4Pin, INPUT);
	//	pinMode(Flex5Pin, INPUT);
	flexValueArr[2] = 500;
	flexValueArr[3] = 600;
	flexValueArr[4] = 700;
	filteredvalue[0] = analogRead(Flex1Pin);
	filteredvalue[1] = analogRead(Flex2Pin);
	filteredvalue[2] = 500;
	filteredvalue[3] = 600;
	filteredvalue[4] = 700;
}
bool DataHandler::isready(int* ptr) {
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
uint8_t*  DataHandler::getFlexData() {
	// �÷��� ������ �����͸� �޾� ���� ���� �� int�� �迭(angle��)�� ��ȯ.
	flexValueArr[0] = analogRead(Flex1Pin); //�÷�������
	flexValueArr[1] = analogRead(Flex2Pin); //��������


	/*flexValueArr 2 ~ 4���� ���� ������ �ȴ޾Ƽ� �׳� ��� �������� �� �־��ִ� �ڵ� */
	int dir2;
	if (flexValueArr[2] >= 800)
		dir2 = -1;
	else if (flexValueArr[2] <= 500)
		dir2 = 1;
	flexValueArr[2] += dir2;

	int dir3;
	if (flexValueArr[3] >= 800)
		dir3 = -2;
	else if (flexValueArr[3] <= 500)
		dir3 = 2;
	flexValueArr[3] += dir3;

	int dir4;
	if (flexValueArr[4] >= 800)
		dir4 = -5;
	else if (flexValueArr[4] <= 500)
		dir4 = 5;
	flexValueArr[4] += dir4;

	filtFlexData(); // ���͸�
	return anglevalue; // ������ �迭 �ּҰ� ��ȯ
}

void DataHandler::filtFlexData() {
	// �÷��� ������ �����͸� �迭�� ���޹޾� low pass filter�� �����ϰ� 50 ~ 180�� ������ ��ȯ.
	// ��ȯ�� ���� uint8_t �迭�� ��ȯ. ��ȯ�ϴ� �迭�� ���̴� 5

	for (int i = 0; i < 5; i++) {
		filteredvalue[i] = filteredvalue[i] * (1 - alpha) + flexValueArr[i] * alpha; //���� ����
		delay(10);

		//���� ���� (50~110 degree)  //�Ƹ� ������ �� �޶� max�� min�� �����ѵڿ� ������ �迭�� ���ؼ� �������
		if (filteredvalue[i] <= 500) filteredvalue[i] = 500;
		else if (filteredvalue[i] >= 800) filteredvalue[i] = 800;
		anglevalue[i] = map((int)filteredvalue[i], 500, 800, 50, 180); //������ ��ȯ
		anglevalue[i] *= -1; // 180 ~ 50 ������ 50 ~ 180 ���� ��ȯ����
		anglevalue[i] += 230;
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

