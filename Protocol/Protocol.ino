#include <Wire.h>
#include <Servo.h>
#include <stdint.h>

const int butPin1 = 6;
const int butPin2 = 7;
const int MPU=0x68;

class Flex{
  private:
    uint16_t value[5];
  public:
    void Receive(); // FlexSensor -> Arduino
    void Send(); // Arduino -> Unity

  Flex(){
  }

};

void Flex::Receive() // FlexSensor -> Arduino
{
  for(int i = 0 ; i < 5 ; i ++ )
  {
    this->value[i] = analogRead(i); 
  }
}

void Flex::Send() // Arduino -> Unity
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
      Wire.endTransmission(false);
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

void Gyro::Receive() // Gyro Sensor -> Arduino 
{
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}
void Gyro::Send() //Arduino -> Unity 
{
  Serial.write(AcX);
  Serial.write(AcY);
  Serial.write(AcZ);
  Serial.write(Tmp/340+36);  
  Serial.write(GyX);
  Serial.write(GyY);
  Serial.write(GyZ);
  delay(333);
}

class gloVR_Servo{
  private:
    uint16_t deg;
  public:
    void ch_deg(); // 각도 바꾸는 함수 

    gloVR_Servo(){
      deg = 0;
      Servo servo;
    }
};

void gloVR_Servo::ch_deg() // 각도 바꾸는 함수 
{
  
}

class Data{
  private:
    // 플렉스센서값, 자이로센서값, 영상처리 값 배열로 받자.
  public:
  
  
};

class SetArdToUni{
  private:
  public:
    SetArdToUni(){
      Serial.begin(9600);
    }
};

class But{
  private:
  public:
    void Left();
    void Right();
    
    But(){
      pinMode(butPin1, INPUT);
      pinMode(butPin2, INPUT);
  
      digitalWrite(butPin1,HIGH);
      digitalWrite(butPin2,HIGH);
    }
};

void But::Left(){
  Serial.println("LEFT");
  Serial.write(1);
  Serial.flush();
  delay(50);
}

void But::Right(){
  Serial.println("RIGHT");
  Serial.write(2);
  Serial.flush();
  delay(50);
  
}

//플렉스센서, 자이로센서, 서보모터 객체 생성.
/*
Flex flex;
Gyro gyro;
Survo survo[5];
*/

// 버튼 객체 생성
But but;

void setup() {
  // put your setup code here, to run once:
  SetArdToUni setardtouni = SetArdToUni(); // 시리얼 통신 시작
  
  //플렉스센서, 자이로센서, 서보모터 객체들의 각 값들 초기화.
/*
  flex = Flex();
  survo[5] = { Survo(), Survo(), Survo(), Survo(), Survo() };
  gyro = Gyro();
*/

  but = But();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  // 버튼 테스트 ( 아두이노 -> 유니티 ) 
  if(digitalRead(butPin1) == LOW){
    but.Left();
  }
  if(digitalRead(butPin2) == LOW){
    but.Right();
  }
 
}
