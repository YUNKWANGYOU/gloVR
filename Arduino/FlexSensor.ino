#include <SoftwareSerial.h>
#define dataArrayLen 25

const int buttonPin01 = 6;
const int buttonPin02 = 7;
const int ledPin01 = 9;
const int FLEX_PIN = A0;

//==============filter활용 변수=========//
//low pass filter
float flexvalue; 
float filteredvalue;//필터값
float alpha = 0.2; //민감도

//moving average filter
#define num 15
int angle;
int anglearray[num];
int filteredangle;
//===================================//

SoftwareSerial mySerial(3,2);


class DataHandler{
  private:
  char arduinoToUnityDataArray[dataArrayLen];
  // start record(0), Array Length(1), flex Sensor(2 ~ 11), zyro Sensor(12 ~ 23), end record(24)

  char unityToArduinoDataArray[dataArrayLen];
  // start record(0), Array Length(1), Command(2), param(3~7), end record(8)
  
  public:
  DataHandler(){
    arduinoToUnityDataArray[0] = 0x7E;
    arduinoToUnityDataArray[1] = dataArrayLen;
    arduinoToUnityDataArray[24] = 0x7D;

    unityToArduinoDataArray[0] = 0x7E;
    unityToArduinoDataArray[1] = dataArrayLen;
    unityToArduinoDataArray[9] = 0x7D;
  }

  int send(SoftwareSerial mySerial);
//  int receive(SoftwareSerial mySerial){
//
//    int returnValue;
//    
//    if(mySerial.available()){
//      returnValue = mySerial.readBytes(unityToArduinoDataArray,dataArrayLen);
//    }
//    
//    return returnValue;
//  }

  int setSendData(uint16_t * sensorData){
    int i=0;
    char * data = (char*)sensorData;
    
    for(i=2;i<24;i++){
      arduinoToUnityDataArray[i] = data[i%2];
    }
    return 1;
  }

  int checkData(char * receiveDataArray){
    // need
    return 0;
  }
  
};

  
int DataHandler::send(SoftwareSerial mySerial){

    int returnValue;
    returnValue = mySerial.write(arduinoToUnityDataArray,dataArrayLen);
    
    return returnValue;
  }

int getDataFromSensor(){
  
}

DataHandler handle;

int writeArray(SoftwareSerial mySerial,uint16_t * arr,int arrLen){
  int i=0;
  for(i=0;i<arrLen;i++){
    mySerial.write(arr[i]);
  }
  mySerial.write('\n');

  return 1;
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(buttonPin01,INPUT);
  pinMode(buttonPin02,INPUT);
  pinMode(ledPin01,OUTPUT);
  pinMode(FLEX_PIN, INPUT);

  digitalWrite(buttonPin01,HIGH);
  digitalWrite(buttonPin02,HIGH);
  digitalWrite(ledPin01,HIGH);
  int i = 1;
  Serial.print("i % 2 == ");
  Serial.println(i%2);

//==================================
  flexvalue =  analogRead(FLEX_PIN);
  filteredvalue = flexvalue;
//================================== 
}

void loop() {
   //uint16_t flexADC = (analogRead(FLEX_PIN) - 800);

//========================================================================================
  //low pass filter로 센서값 필터링
  flexvalue = analogRead(A0);
  filteredvalue = filteredvalue * (1 - alpha) + flexvalue * alpha; //필터 계산식
  delay(10);
  /*Serial.println(filteredvalue);
  Serial.print(",");*/
  //Serial.println(flexvalue);

  //각도 범위 지정 (50~110 degree)
 angle = map((int)filteredvalue%100,18, 23, 50, 120); //각도로 변환
  angle *= -1;
  angle += 160;
  if(angle <= 50) angle = 50;
  else if (angle >= 110) angle = 110;

  //moving average filter로 각도값 필터링
  for(int i = 0; i<num-1; i++){
    anglearray[i] = anglearray[i+1];
  }
  anglearray[num -1] = angle;
  for(int i = 0; i<num-1; i++){
    filteredangle += anglearray[i];
  }
  filteredangle /= num;
  uint16_t flexADC = filteredangle; // 최종값 저장
//==========================================================================================
  Serial.print("flexData : ");
  Serial.println(flexADC);
  char * data = (char*)&flexADC;

  uint16_t testData1[12] = {flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC};
  uint16_t testData2[12] = {flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC,flexADC};

  if(digitalRead(buttonPin02) == LOW){
    writeArray(mySerial,testData1,12);
    delay(100);
  }

  if(digitalRead(buttonPin01) == LOW){
    writeArray(mySerial,testData2,12);
    delay(100);
  }
  
//  handle.setSendData(&flexADC);
//  Serial.println(handle.send(mySerial));
//  delay(1000);
  
  if(mySerial.available()){
    Serial.write(mySerial.read());
  }

  if(Serial.available()){
    Serial.println("-");
    mySerial.write(Serial.read());
  }

}
