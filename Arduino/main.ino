 #include <SoftwareSerial.h>
#define dataArrayLen 25

const int buttonPin01 = 6;
const int buttonPin02 = 7;
const int ledPin01 = 9;
const int FLEX_PIN = A0;

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
}

void loop() {
//  uint16_t flexADC = (analogRead(FLEX_PIN) - 800);
  uint16_t flexADC = analogRead(FLEX_PIN);
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