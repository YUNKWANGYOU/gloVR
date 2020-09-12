#include <DataHandler.h>

DataHandler test(2,3);
uint8_t *flexData;
void setup() 
{ 
  Serial.begin(9600);
  test.InitFlex();
  test.FilterDeg(0.5);
} 

void loop() 
{ 
  //플로터 출력을 위한 임시 코드
 flexData = test.getFlexData();
 int data0 = *(flexData+0);
 int data1 = *(flexData+1);
 int data2 = *(flexData+2);
 int data3 = *(flexData+3);
 int data4 = *(flexData+4);
 Serial.print(data0);
 Serial.print(' '); 
 Serial.print(data1); 
 Serial.print(' ');
 Serial.print(data2); 
 Serial.print(' ');
 Serial.print(data3); 
 Serial.print(' ');
 Serial.println(data4); 
}
