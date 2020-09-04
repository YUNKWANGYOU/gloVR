#include <SoftwareSerial.h>

// 자이로
// INT -> D12
// SDA -> A4
// SCL -> A5

// 플렉스
// 1 -> A0
// 2 -> A1
// 3 -> A2
// 4 -> A3
// 5 -> A6

// 서보
// 1 -> D5
// 2 -> D6
// 3 -> D9
// 4 -> D10
// 5 -> D11

// 블루투스
// RX -> TX
// TX -> RX

//FlexSensor pin number
#define Flex1Pin 0;
#define Flex2Pin 3;
#define Flex3Pin 4;
#define Flex4Pin 5;
#define Flex5Pin 6;

//ZyroSensor pin number
// #define ZyroPin 

//Bluetooth module pin number;
#define BluetoothRxPin 3;
#define BluetoothTxPin 2;

//Survo motor pin number;
#define Survo1Pin 10;
#define Survo2Pin 11;
#define Survo3Pin 12;
#define Survo4Pin 13;
#define Survo5Pin 14;

//define data len
#define sendDataArrayLen 24
#define receiveDataArrayLen 12

class DataHandler{

    private:
    //arduino -> unity data
    // start record(0), Array Length(1), flex Sensor(2 ~ 11), zyro Sensor(12 ~ 23), end record(24)
    char arduinoToUnityDataArray[sendDataArrayLen];
    
    //unity -> arduino data
    // start record(0), Array Length(1), Command(2), param(3~7), end record(8)
    char unityToArduinoDataArray[receiveDataArrayLen];
    
    //bluetooth module
    SoftwareSerial mySerial(BluetoothRxPin,BluetoothTxPin);

    public:

    // 초기화하기
    bool InitFlex();
    bool InitZyro();
    bool InitSurvo();
    bool InitBluetooth();

    //데이터 송신시 사용하는 메소드
    char getFlexData(int * intArr);
    char getZyroData(int * intArr);
    char setAllSendData(int * intArr);
    bool sendData();

    //데이터 수신시 사용하는 메소드
    int * ReceiveData();
    bool RotateSurvo();

}