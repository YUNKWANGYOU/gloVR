#include <SoftwareSerial.h>
#include <Servo.h>
 
#define BT_RXD 6
#define BT_TXD 7
#define ServoPin 8

SoftwareSerial bluetooth(BT_RXD, BT_TXD);
Servo servo1;

int angle = 0;

byte byteArr[7];
int i=0;
 
void setup(){
  Serial.begin(9600);
  bluetooth.begin(9600);

  servo1.attach(ServoPin);
}
 
void loop(){
  if (bluetooth.available()) {
    byte readData = bluetooth.read();
    Serial.write(readData);
    Serial.print("/");
    byteArr[i++] = readData;
  }

  if(i == 7){
    i = 0;
    Serial.println("all done");
    if(checkData(byteArr) == 0){
      Serial.print("잘못된 데이터입니다!");
    }

    if(byteArr[1] == '0'){
      servo1.write(0);
    }

    if(byteArr[1] == '1'){
      servo1.write(60);
    }
    
    if(byteArr[1] == '2'){
      servo1.write(120);
    }
    
    if(byteArr[1] == '3'){
      servo1.write(150);
    }
    clearArr(byteArr);
    
  }
  
  if (Serial.available()) {
    bluetooth.write(Serial.read());
  }

  
}

int checkData(byte * ar){
  if(ar[0] != 's')
    return 0;

  if(ar[6] != 'e')
    return 0;

   return 1;
}

int clearArr(byte * ar){
  int k=0;
  for(k=0;k<7;k++){
    ar[k] = 0;
  }
}