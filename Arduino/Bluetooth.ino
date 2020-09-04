#include <SoftwareSerial.h>

// 1번 버튼을 누르면 블루투스 모듈에서 데이터를 전송.
// 2번 버튼을 누르면 블루투스 모듈에서 데이터를 받아 serial모니터에 출력.

const int RxPin = 3;
const int TxPin = 2;
const int buttonPin01 = 6;
const int buttonPin02 = 7;

SoftwareSerial mySerial(RxPin,TxPin);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(buttonPin01,INPUT);
  pinMode(buttonPin02,INPUT);

  digitalWrite(buttonPin01,HIGH);
  digitalWrite(buttonPin02,HIGH);
}

void loop() {

  if(digitalRead(buttonPin01) == LOW){
    mySerial.write(1)
    delay(100);
  }

  if(digitalRead(buttonPin02) == LOW){
    if(mySerial.available()){
        Serial.write(mySerial.read());
    }
  }

}
