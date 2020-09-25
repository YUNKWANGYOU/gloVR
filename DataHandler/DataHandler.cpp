
// include this library's description file
#include "MPU6050_6Axis_MotionApps20.h"
#include "DataHandler.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "I2Cdev.h"
#include "Wire.h"
#include "String.h"


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


void DataHandler::dmpDataReady() {
	this->mpuInterrupt = true;
}

void (DataHandler::*fcnPtr)() = &DataHandler::dmpDataReady;

void DataHandler::InitZyro() {

	Wire.begin();
	Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
	//MPU 6050 ���� �ʱ�ȭ

	mpu.initialize();
	//���ͷ�Ʈ��(2) �Է����� ����
	pinMode(INTERRUPT_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), fcnPtr, RISING);

	//DMP �ʱ�ȭ
	//DMP�� MPU6050���ο� �ִ� Digital Motion Processor

	devStatus = mpu.dmpInitialize();

	// �ʱ� ���� ����.... �ϴ� �״�� �ΰ� �н�
	mpu.setXGyroOffset(220);
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1788);

	//�ʱ�ȭ�� �ߵǾ��ٸ�?
	if (devStatus == 0) {

		//DMP Ȱ��ȭ
		Serial.println(F("Enabling DMP..."));
		mpu.setDMPEnabled(true);
		mpuIntStatus = mpu.getIntStatus();
		dmpReady = true;
		// FIFO ��Ŷ ������ ������
		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else {
	}
}


void DataHandler::InitServo() {

	servoArr[0].attach(servo0Pin);
	servoArr[1].attach(servo1Pin);
	servoArr[2].attach(servo2Pin);
	servoArr[3].attach(servo3Pin);
	servoArr[4].attach(servo4Pin);

	for (int i = 0; i < 5; i++) {
		servoArr[i].write(0);
	}
}
/*
void  DataHandler::GetFlexRange() {
	delay(2000);
	*flexMax = *filteredValue;
	*flexMin = *filteredValue;
	for (int i = 0; i < 5000; i++) {
		for (int j = 0; j < 5; j++) {
			flexMax[j] = max(flexMax[j], filteredValue[j]);
			flexMin[j] = min(flexMin[j], filteredValue[j]);
		}
	}
}
*/
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
		if (filteredValue[i] <= flexMin[i]) filteredValue[i] = flexMin[i];
		else if (filteredValue[i] >= flexMax[i]) filteredValue[i] = flexMax[i];
		angleValue[i] = map((int)filteredValue[i], flexMin[i], flexMax[i], 50, 180); //������ ��ȯ
		angleValue[i] *= -1; // 180 ~ 50 ������ 50 ~ 180 ���� ��ȯ����
		angleValue[i] += 230;
	}
}

uint8_t* DataHandler::GetZyroData() {
	// ���̷� ������ �����͸� �޾� �迭�� ��ȯ.
	   // ������ �ʱ�ȭ�� �� �ȵƴٸ� �׳� �Լ� ����
	if (!dmpReady) return;

	// ������ ���ͷ�Ʈ�� ����� �Լ����� mpuInterrupt������ �����ϴµ�
	// �̰����� �� ������ ��ٸ��ٰ� ���ͷ�Ʈ�� �߻��ϸ� �������� �Ѿ�� 
	// �����Ǿ� �ִ�.
	while (!mpuInterrupt && fifoCount < packetSize);

	// ���ͷ�Ʈ ���� �ʱ�ȭ
	mpuInterrupt = false;
	// mpu6050 ���� �б�
	mpuIntStatus = mpu.getIntStatus();

	// FIFO ���� ���� ���
	fifoCount = mpu.getFIFOCount();

	//  fifo�� ���ƴٸ�?
	if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
		mpu.resetFIFO();
		Serial.println(F("FIFO overflow!"));
	}
	else if (mpuIntStatus & 0x02) {
		// packetSize��ŭ fifo�� ���ö����� ���              
		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

		// ������ �޾� ����
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		fifoCount -= packetSize;

		//�� ������
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
	}

	return FiltZyroData();

}

uint8_t* DataHandler::FiltZyroData() {
	// ���̷� ������ �����͸� �迭�� �޾� 00���͸� �����ϰ� 00 ~ 000�� ������ ��ȯ.
	// ��ȯ�� ���� uint8_t �迭�� ��ȯ. ��ȯ�ϴ� �迭�� ���̴� 5

	uint8_t floatToIntArr[6];
	floatToIntArr[0] = floatToInt(ypr[0]) / 100;
	floatToIntArr[1] = floatToInt(ypr[0]) % 100;
	floatToIntArr[2] = floatToInt(ypr[1]) / 100;
	floatToIntArr[3] = floatToInt(ypr[1]) % 100;
	floatToIntArr[4] = floatToInt(ypr[2]) / 100;
	floatToIntArr[5] = floatToInt(ypr[2]) % 100;

	return floatToIntArr;
}

uint16_t DataHandler::floatToInt(float ypr){
	return (uint16_t)((ypr + 4) * 100);
}

void  DataHandler::SetSendData(uint8_t* flexDataArr, uint8_t* zyroDataArr) {
	// ���Ͱ� ����� flexDataArr�� zyroDataArr�� ���޹޾� 
	// arduinoToUnity�迭�� ����.
	// flexDataArr�� ���̴� 5, zyroDataArr�� ���̴� 6
	int i=0;
	for(i=0;i<5;i++){
		arduinoToUnityDataArray[i+1] = flexDataArr[i];
	}

	for(i=0;i<6;i++){
		arduinoToUnityDataArray[i+6] = zyroDataArr[i];
	}
}

void DataHandler:: CheckAllSendData() {
	// ������ �� arduinoToUnityDataArray�迭�� ������ �˻�.
}

void DataHandler::SendData() {
	// arduinoToUnityDataArray�� �������� ����� ���ؼ� ����.
	int i = 0;

	for (i = 0; i < 13; i++) {
		mySerial.write(arduinoToUnityDataArray[i]);
	}
}

//������ ���Ž� ����ϴ� �޼ҵ�
uint8_t* DataHandler::ReceiveData() {
	// �������� ����� ���ؼ� �����͸� �޾� unityToArduinoDataArray �迭�� ����.

}

void DataHandler::RotateServo() {
	// unityToArduinoDataArray�迭�� �����Ϳ� ���� �������� ������ ȸ��.
}

