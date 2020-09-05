#include <SoftwareSerial.h>
#include <Servo.h>

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
#define sendDataArrayLen 13
#define receiveDataArrayLen 7

// filter 민감도
#define alpha 0.2

class DataHandler{

    private:
    //arduino -> unity data
    // start record(0), flex Sensor(1 ~ 5), zyro Sensor(5 ~ 11), end record(12)
    char arduinoToUnityDataArray[sendDataArrayLen];
    
    //unity -> arduino data
    // start record(0), param(1~5), end record(6)
    char unityToArduinoDataArray[receiveDataArrayLen];
    
    //bluetooth module
    SoftwareSerial mySerial(BluetoothRxPin,BluetoothTxPin);

    //servo motor array
    Servo servoArr[5];

    //flexSensor filter에 필요한 값
    int flexValueArr[5];


    public:
    // 초기화하기
    bool InitFlex();
    bool InitFlexFilter();
    bool InitZyro();
    bool InitZyroFilter();
    bool InitSurvo();
    bool InitBluetooth();

    //데이터 송신시 사용하는 메소드
    int* getFlexData();
    uint8_t* filtFlexData(int* dataArr);

    int* getZyroData();
    uint8_t* filtZyroData(int* dataArr);

    bool setSendData(uint8_t* flexDataArr, uint8_t* zyroDataArr);

    char checkAllSendData(int * intArr);
    
    bool sendData();

    //데이터 수신시 사용하는 메소드
    int * ReceiveData();
    bool RotateSurvo();

}
