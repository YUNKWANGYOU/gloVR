#include <SoftwareSerial.h>

const int RxPin = 6;
const int TxPin = 7;

SoftwareSerial mySerial(RxPin,TxPin);
uint8_t SendData[6];
bool up = false;
int i=0;
int j=0;


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.print("serial start");
  InitSendData();
}

void InitSendData(){
  SendData[0] = 1;
  SendData[1] = 46;
  SendData[2] = 4;
  SendData[3] = 40;
  SendData[4] = 3;
  SendData[5] = 59;
}

void loop() {
  
  
  if(up == true){
    SendData[1]++;
    SendData[3]++;
    SendData[5]++;
    i++;
  }
  else{
    SendData[1]--;
    SendData[3]--;
    SendData[5]--;
    i++;
  }

  if(i == 40){
    i=0;
    if(up == true){
      up = false;
    }
    else{
      up = true;
    }
  }

  mySerial.write('{');
  for(j=0;j<6;j++){
    mySerial.write(SendData[j]);
    Serial.print(SendData[j]);
    Serial.print(" ");
  }
  mySerial.write('}');

  Serial.println();
  Serial.println("SendSuccess!");
}
