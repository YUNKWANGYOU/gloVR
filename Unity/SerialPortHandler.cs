using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class SerialPortHandler : MonoBehaviour
{
    SerialPort sp = new SerialPort("/dev/tty.PARK-DevB",9600);

    void Awake(){
        DontDestroyOnLoad(gameObject);
    }
    // Start is called before the first frame update
    void Start()
    {
        //port open
        sp.ReadTimeout = 100;
        sp.Open();
        print("Serial ports open");   
    }

    // Update is called once per frame
    // void Update()
    // {
    //     if(!sp.IsOpen){
	// 		Debug.Log("port close");
	// 	}    
    // }

    public bool SendString(string string_data){
        //문자열 검사하는 코드 작성할건지?
        if(sp.IsOpen){
            sp.Write(string_data);
            return true;
        }
        else{
            // throw Exception;
            return false;
        }
    }

    // public string ReceiveString(){
    //     if(sp.IsOpen){
    //         try{
    //             string recv_data = sp.ReadLine();
    //             Debug.Log(recv_data);
    //             return recv_data;
    //         }
    //         catch(System.Exception e){
    //             Debug.Log(e);
    //         }
    //     }
    //     else{
    //         return "0";
    //     }
    // }

    public bool IsConnected(){
        int b = 0;
        
        if(sp.IsOpen){
            try{                
                b = sp.ReadByte();
            }
            catch(System.Exception e){
                Debug.Log(e);
            }
            if(b != 0){
                return true;
            }
        }

        return false;
    }

    public bool ReceiveArduinoData(ref int[] flex_data,ref float[] zyro_data){
        int start_byte = 0;
        int end_byte = 0;

        int i = 0;
        string receiveData1 = "";
        string receiveData2 = "";
        string receiveData3 = "";
        float yaw;
        float pitch;
        float roll;


        if(sp.IsOpen){
            try{
                start_byte = sp.ReadByte();
            }
            catch(System.Exception e){
                Debug.Log(e);
            }

            if(start_byte == 200){
                for(i=0;i<5;i++){
                    flex_data[i] = sp.ReadByte();
                }

                receiveData1 = sp.ReadLine();
                receiveData2 = sp.ReadLine();
                receiveData3 = sp.ReadLine();
                
                end_byte = sp.ReadByte();
            }

            if(end_byte == 201){
                yaw = float.Parse(receiveData1);
                pitch = float.Parse(receiveData2);
                roll = float.Parse(receiveData3);

                yaw = (float)(yaw * 180 / 3.14);
                pitch = (float)(pitch * 180 / 3.14);
                roll = (float)(roll * 180 / 3.14);

                zyro_data[0] = yaw;
                zyro_data[1] = pitch;
                zyro_data[2] = roll;

                return true;
            }
        }

        return false;

    }

}


// public class receiveData : MonoBehaviour
// {
//     SerialPort sp = new SerialPort("/dev/tty.PARK-DevB",9600);

//     string receiveData1;
// 	string receiveData2;
// 	string receiveData3;

//     float yaw;
//     float roll;
//     float pitch;

//     public GameObject hand;
//     // handMotion = hand.GetComponent<hand_motion>();

//     // Start is called before the first frame update
//     void Start()
//     {
//         //port open
//         sp.ReadTimeout = 50;
//         sp.Open();
//         print("Serial ports open");
//     }

//     // Update is called once per frame
//     void Update()
//     {
//         //---------------------------byte로 주고받는거 첫번째 시도--------------------------------------------
// 		// int readNum = 0;
// 		// // hand ratate 
// 		// if(sp.IsOpen){
// 		// 	try{
// 		// 		readNum = sp.Read(recvArr,0,numData);
// 		// 	}
// 		// 	catch(System.Exception e){
// 		// 		Debug.Log(e);
// 		// 	}
// 		// }

// 		// if(readNum == 6){
// 		// 	print("recvArr : " + recvArr[0] + " " + recvArr[1]+ " " + recvArr[2]+ " " + recvArr[3]+ " " + recvArr[4]+ " " + recvArr[5]);
// 		// 	int[] intArr = byteArrToIntArr(recvArr);
// 		// 	print("intArr : " + intArr[0] + " " + intArr[1]+ " " + intArr[2]+ " " + intArr[3]+ " " + intArr[4]+ " " + intArr[5]);

// 		// 	float yaw = intToFloat(intArr[0],intArr[1]);
// 		// 	float roll = intToFloat(intArr[2],intArr[3]);
// 		// 	float pitch = intToFloat(intArr[4],intArr[5]);
// 		// 	print("yaw1 : " + (yaw) + " roll1 : " + (roll) + " pitch1 : " + (pitch));

// 		// 	yaw = (float)(yaw * 180 / 3.14);
// 		// 	roll = (float)(roll * 180 / 3.14);
// 		// 	pitch = (float)(pitch * 180 / 3.14);
// 		// 	print("yaw : " + (yaw) + " roll : " + (roll) + " pitch : " + (pitch));

// 		// 	hand.transform.rotation = Quaternion.Euler(yaw,roll,pitch);
// 		// }

// 		//---------------------------byte로 주고받는거 두번째 시도--------------------------------------------
// 		// if(sp.IsOpen){
// 		// 	try{
// 		// 		recvByteData = (byte)sp.ReadByte();
// 		// 		// Debug.Log(recvByteData);
// 		// 	}
// 		// 	catch(System.Exception e){
// 		// 		Debug.Log(e);
// 		// 	}

// 		// 	if(recvInProccess == true){
// 		// 		if(recvByteData != 125){
// 		// 			if(ndx >= numData){
// 		// 				ndx = 0;
// 		// 				recvInProccess = false;
// 		// 			}
// 		// 			else{
// 		// 				recvArr[ndx] = recvByteData;
// 		// 				ndx++;
// 		// 			}
// 		// 		}
// 		// 		else{
// 		// 			recvInProccess = false;
// 		// 			ndx = 0;
// 		// 			newData = true;
// 		// 		}
// 		// 	}
// 		// 	else if(recvByteData == 123){
// 		// 		recvInProccess = true;
// 		// 	}
// 		// }

// 		// print(newData);


// 		// if(newData == true){
// 		// 	ShowData();
// 		// }
// 		//----------------------------------------------문자열로 주고받기-----------------------------------------
// 		if(sp.IsOpen){
// 			try{
// 				receiveData1 = sp.ReadLine();
// 				receiveData2 = sp.ReadLine();
// 				receiveData3 = sp.ReadLine();
// 			}
// 			catch(System.Exception e){
// 				Debug.Log(e);
// 			}
// 		}

// 		Debug.Log("——————————");
// 		Debug.Log(receiveData1);
// 		Debug.Log(receiveData2);
// 		Debug.Log(receiveData3);
// 		Debug.Log("——————————");

// 		float yaw = float.Parse(receiveData1);
// 		float roll = float.Parse(receiveData2);
// 		float pitch = float.Parse(receiveData3);

// 		yaw = (float)(yaw * 180 / 3.14);
// 		roll = (float)(roll * 180 / 3.14);
// 		pitch = (float)(pitch * 180 / 3.14);

// 		hand.transform.rotation = Quaternion.Euler(yaw,roll,pitch);
//     }
// }
