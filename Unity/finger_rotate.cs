// 플렉스 센서를 통한 hand 제어.
// 검지손가락만 움직이게 해놨음.
// 방향키로 손가락 움직이는거 여전히 가능
// 블루투스 모듈과 잘 연결이 되야함

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class finger_rotate : MonoBehaviour
{

    // Serial port Object
	SerialPort sp = new SerialPort("/dev/tty.PARK-DevB",9600);
	int degree = 0;
	int status = 0;
	int[] recvData = new int[5];
	// hand
	private GameObject hand;

	//finger
	private GameObject finger_1;
	private GameObject finger_2;
	private GameObject finger_3;

	// thumb variable
	private GameObject thumb_1;
	private GameObject thumb_2;

	// index finger variable
	private GameObject index_finger_1;
	private GameObject index_finger_2;
	private GameObject index_finger_3;

	// middle finger variable
	private GameObject middle_finger_1;
	private GameObject middle_finger_2;
	private GameObject middle_finger_3;

	// ring finger variable
	private GameObject ring_finger_1;
	private GameObject ring_finger_2;
	private GameObject ring_finger_3;

	// pinky variable
	private GameObject pinky_1;
	private GameObject pinky_2;
	private GameObject pinky_3;

	// Start is called before the first frame update
	void Start()
    {
        // open port
		print("START!\n");
		sp.Open();
		sp.ReadTimeout = 500;
		print("port open");
	
		// read all Children of current object
		Transform[] allChildren = GetComponentsInChildren<Transform>();

		// iterate
		foreach(Transform child in allChildren)
		{
			// find hand object
			if (child.name == "Hand")
				hand = child.gameObject;

			// find thumb object
			if (child.name == "thumb_1")
				thumb_1 = child.gameObject;
			if (child.name == "thumb_2")
				thumb_2 = child.gameObject;

			// find index finger object
			if (child.name == "index_finger_1")
				index_finger_1 = child.gameObject;
			if (child.name == "index_finger_2")
				index_finger_2 = child.gameObject;
			if (child.name == "index_finger_3")
				index_finger_3 = child.gameObject;

			// find middle finger object
			if (child.name == "middle_finger_1")
				middle_finger_1 = child.gameObject;
			if (child.name == "middle_finger_2")
				middle_finger_2 = child.gameObject;
			if (child.name == "middle_finger_3")
				middle_finger_3 = child.gameObject;

			// find ring finger object
			if (child.name == "ring_finger_1")
				ring_finger_1 = child.gameObject;
			if (child.name == "ring_finger_2")
				ring_finger_2 = child.gameObject;
			if (child.name == "ring_finger_3")
				ring_finger_3 = child.gameObject;

			// find pinky object
			if (child.name == "pinky_1")
				pinky_1 = child.gameObject;
			if (child.name == "pinky_2")
				pinky_2 = child.gameObject;
			if (child.name == "pinky_3")
				pinky_3 = child.gameObject;
		}

		// set default finger as index finger
		finger_1 = index_finger_1;
		finger_2 = index_finger_2;
		finger_3 = index_finger_3;
	}

	void RotateFinger(int[] intDataArr){
		int[] rotate_degree = new int[5];
		for(int i=0;i<5;i++){
			rotate_degree[i] = 180 - intDataArr[i];
		}

		index_finger_1.transform.localEulerAngles = new Vector3(rotate_degree[3] * 2,0,0) * 0.3f;
		index_finger_2.transform.localEulerAngles = new Vector3(rotate_degree[3] * 2,0,0) * -0.9f;
		index_finger_3.transform.localEulerAngles = new Vector3(rotate_degree[3] * 2,0,0) * -0.9f;
		
		middle_finger_1.transform.localEulerAngles = new Vector3(rotate_degree[2] * 2,0,0) * 0.3f;
		middle_finger_2.transform.localEulerAngles = new Vector3(rotate_degree[2] * 2,0,0) * -0.9f;
		middle_finger_3.transform.localEulerAngles = new Vector3(rotate_degree[2] * 2,0,0) * -0.9f;

		ring_finger_1.transform.localEulerAngles = new Vector3(rotate_degree[1] * 2,0,0) * 0.3f;
		ring_finger_2.transform.localEulerAngles = new Vector3(rotate_degree[1] * 2,0,0) * -0.9f;
		ring_finger_3.transform.localEulerAngles = new Vector3(rotate_degree[1] * 2,0,0) * -0.9f;

		pinky_1.transform.localEulerAngles = new Vector3(rotate_degree[0] * 2,0,0) * 0.3f;
		pinky_2.transform.localEulerAngles = new Vector3(rotate_degree[0] * 2,0,0) * -0.9f;
		pinky_3.transform.localEulerAngles = new Vector3(rotate_degree[0] * 2,0,0) * -0.9f;
	}

    // Update is called once per frame
    void Update()
    {
        //translate and rotate
		if(sp.IsOpen){
			try{
			// RotateFinger(sp.ReadByte());
				for(int i=0;i<5;i++){
					recvData[i] = sp.ReadByte();
					Debug.Log(recvData[i]);
				}

			}
			catch(System.Exception e){
				Debug.Log(e);
			}
     	}
		 RotateFinger(recvData);

		// change finger (thumb)
		if (Input.GetKeyDown(KeyCode.Alpha1))
		{
			finger_1 = thumb_1;
			finger_2 = thumb_2;
			finger_3 = null;
		}
		// change finger (index finger)
		if (Input.GetKeyDown(KeyCode.Alpha2))
		{
			finger_1 = index_finger_1;
			finger_2 = index_finger_2;
			finger_3 = index_finger_3;
		}
		// change finger (middle finger)
		if (Input.GetKeyDown(KeyCode.Alpha3))
		{
			finger_1 = middle_finger_1;
			finger_2 = middle_finger_2;
			finger_3 = middle_finger_3;
		}
		// change finger (ring finger)
		if (Input.GetKeyDown(KeyCode.Alpha4))
		{
			finger_1 = ring_finger_1;
			finger_2 = ring_finger_2;
			finger_3 = ring_finger_3;
		}
		// change finger (pinky)
		if (Input.GetKeyDown(KeyCode.Alpha5))
		{
			finger_1 = pinky_1;
			finger_2 = pinky_2;
			finger_3 = pinky_3;
		}


		//Vector3 angle = Quaternion.Euler(0f, 3f, 0f);
		if (Input.GetKey(KeyCode.UpArrow) && finger_1 != thumb_1)
		{
			//Quaternion current_angle = index_finger_1.transform.rotation;
			//Quaternion angle = current_angle + new Quaternion(0, 3, 0, 0);
			//index_finger_1.transform.rotation = Quaternion.Euler(angle);

			finger_1.transform.Rotate(new Vector3(1, 0, 0) * 0.1f);
			finger_2.transform.Rotate(new Vector3(-1, 0, 0) * 0.3f);
			finger_3.transform.Rotate(new Vector3(-1, 0, 0) * 0.3f);

			// print rotation value
			Debug.Log(finger_1.transform.rotation.x);
			Debug.Log(finger_2.transform.rotation.x);
			Debug.Log(finger_3.transform.rotation.x);
			Debug.Log("--------------------------------------------------");
		}
		if (Input.GetKey(KeyCode.DownArrow) && finger_1 != thumb_1)
		{
			finger_1.transform.Rotate(new Vector3(-1, 0, 0) * 0.1f);
			finger_2.transform.Rotate(new Vector3(1, 0, 0) * 0.3f);
			finger_3.transform.Rotate(new Vector3(1, 0, 0) * 0.3f);

			// print rotation value
			Debug.Log(finger_1.transform.rotation);
			Debug.Log(finger_2.transform.rotation);
			Debug.Log(finger_3.transform.rotation);
			Debug.Log("--------------------------------------------------");
		}

		if (Input.GetKey(KeyCode.UpArrow) && finger_1 == thumb_1)
		{
			finger_1.transform.Rotate(new Vector3(0, 0, 1) * 0.1f);
			finger_2.transform.Rotate(new Vector3(0, 0, 1) * 0.3f);

			// print rotation value
			Debug.Log(finger_1.transform.rotation);
			Debug.Log(finger_2.transform.rotation);
			Debug.Log("--------------------------------------------------");
		}
		if (Input.GetKey(KeyCode.DownArrow) && finger_1 == thumb_1)
		{
			finger_1.transform.Rotate(new Vector3(0, 0, -1) * 0.1f);
			finger_2.transform.Rotate(new Vector3(0, 0, -1) * 0.3f);

			// print rotation value
			Debug.Log(finger_1.transform.rotation);
			Debug.Log(finger_2.transform.rotation);
			Debug.Log("--------------------------------------------------");
		}

		// move hand (up)
		if (Input.GetKey(KeyCode.W))
		{
			hand.transform.position += new Vector3(0, 0.01f, 0);
		}
		// move hand (down)
		if (Input.GetKey(KeyCode.S))
		{
			hand.transform.position += new Vector3(0, -0.01f, 0);
		}
		// move hand (right)
		if (Input.GetKey(KeyCode.D))
		{
			hand.transform.position += new Vector3(0.01f, 0, 0);
		}
		// move hand (left)
		if (Input.GetKey(KeyCode.A))
		{
			hand.transform.position += new Vector3(-0.01f, 0, 0);
		}
	}

}
