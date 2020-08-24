#include <Wire.h>
#include <Servo.h>

class Flex{
  private:
    uint16_t value;
  public:
    void Receive(); // FlexSensor -> Arduino
    void Send(); // Arduino -> Unity

  Flex(){
    uint16_t value[5] = { 0, 0, 0, 0, 0 };
  }

};

void Flex::Receive()
{
  for(int i = 0 ; i < 5 ; i ++ )
  {
    this->value[i] = analogRead(i); // Flex Sensor 값 업데이
  }
}

void Flex::Send()
{
  for(int i = 0 ; i < 5 ; i++ )
  {
    Serial.write(value[i]);
  }
}

class Gyro{
  private:
    uint16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
    
  public:
    void Receive(); // Gyro Sensor -> Arduino
    void Send(); // Arduino -> Unity

    Gyro(){
      Wire.beginTransmission(MPU);
      Wire.write(0x3B);
      Wire.endTransmission(flase);
      Wire.requestFrom(MPU,14,true);
      AcX = 0;
      AcY = 0;
      AcZ = 0;
      Tmp = 0;
      GyX = 0;
      GyY = 0;
      GyZ = 0;
    }

    
};

void Gyro::Receive()
{
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}
void Gyro::Send()
{
  Serial.write(AcX);
  Serial.write(AcY);
  Serial.write(AcZ);
  Serial.write(Tmp/340.00+36.53);  
  Serial.write(GyX);
  Serial.write(GyY);
  Serial.write(GyZ);
  delay(333);
}

class gloVR_Servo{
  private:
    uint16_t deg;
  public:
    void ch_deg();

    gloVR_Servo(){
      deg = 0;
      Servo servo;
    }
};

void Survo::ch_deg() // 각도변
{
  this->servo.write(
}

class Data{
  private:
    // 플렉스센서값, 자이로센서값, 영상처리 값
    int
  public:
  
}

class SetArdToUni{
  private:
  public:

  SetArdToUni(){
    Serial.begin(9600);
  }
};

//플렉스센서, 자이로센서, 서보모터 객체 생성.
Flex flex;
Gyro gyro;
Survo survo[5];

void setup() {
  // put your setup code here, to run once:
  SetArdToUni setardtouni = SetArdToUni(); // 시리얼 통신 시작
  
  //플렉스센서, 자이로센서, 서보모터 객체들의 각 값들 초기화.
  flex = Flex();
  survo[5] = { Survo(), Survo(), Survo(), Survo(), Survo() };
  gyro = Gyro();
  
 }

void loop() {
  // put your main code here, to run repeatedly:
  
  /*bool option_degree;
  int degree[5];

  if(option_degree == true)
  { 
    for(i = 0 ; i < 5 ; i++)
    {
      // 옵션에서 서보모터 값 입력받기.
      degree = Serial.read
      survo[i].degree = 
    } 
  }*/

}
