#include <SoftwareSerial.h>
#define dataArrayLen 13

const int FLEX_PIN = A0;

//low pass filter
float flexvalue; 
float filteredvalue;//필터값
float alpha = 0.2; //민감도

//moving average filter
#define num 15
int angle;
int anglearray[num];
int filteredangle;

SoftwareSerial mySerial(3,2);

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

  pinMode(FLEX_PIN, INPUT);

  flexvalue =  analogRead(FLEX_PIN);
  filteredvalue = flexvalue;

}

void loop() {

  //low pass filter로 센서값 필터링
  flexvalue = analogRead(A0);
  filteredvalue = filteredvalue * (1 - alpha) + flexvalue * alpha; //필터 계산식
  delay(10);

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

  Serial.print("flexData : ");
  Serial.println(flexADC);

}
