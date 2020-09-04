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
// RX -> D2
// TX -> D3

//FlexSensor pin number
#define Flex1Pin A0
#define Flex2Pin A1
#define Flex3Pin A2
#define Flex4Pin A3
#define Flex5Pin A6

//ZyroSensor pin number
#define ZyroSDA A4
#define ZyroSCL A5
#define ZyroINT 12

//Bluetooth module pin number
#define BluetoothRxPin 2
#define BluetoothTxPin 3

//Survo motor pin number
#define Survo1Pin 5
#define Survo2Pin 6
#define Survo3Pin 9
#define Survo4Pin 10
#define Survo5Pin 11

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
