#include "DataHandler.h"

DataHandler test(6,7);
uint8_t *zyroData;

void setup() {
  Serial.begin(115200);
  test.InitZyro();

}

void loop() {
  zyroData = test.GetZyroData();

  Serial.print("yaw : ");
  Serial.print(zyroData[0]);
  Serial.print(zyroData[1]);
  Serial.print("\t");
  Serial.print("pitch : ");
  Serial.print(zyroData[2]);
  Serial.print(zyroData[3]);
  Serial.print("\t");
  Serial.print("roll : ");
  Serial.print(zyroData[4]);
  Serial.print(zyroData[5]);
  Serial.print("\t");

}
