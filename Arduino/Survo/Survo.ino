#include <SoftwareSerial.h>
#include <Servo.h>

// 나노 기준 회로설정 사진참고
#define BT_RXD 0
#define BT_TXD 1
#define Servo1 3
#define Servo2 4
#define Servo3 5
#define Servo4 6
#define Servo5 7

// 블루투스, 서보 설정
SoftwareSerial bluetooth(BT_RXD, BT_TXD);
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// 블루투스 수신 값
byte byteArr[7];
int i=0;
 
void setup(){
  Serial.begin(9600);
  bluetooth.begin(9600);

  servo1.attach(Servo1);
  servo2.attach(Servo2);
  servo3.attach(Servo3);
  servo4.attach(Servo4);
  servo5.attach(Servo5);
}
 
void loop(){
  // 블루투스 수신 값 한가지
  // ? 여러가지 받을땐 형식이 어떻게?
  // 수신형태 알려주면 수정하겠음.
  // 수정 필요
  // 집가서 함
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
