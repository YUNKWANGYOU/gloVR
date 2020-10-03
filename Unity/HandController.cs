using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;


public class HandController : MonoBehaviour
{
    //Serial Port Handler
    private SerialPortHandler SPHandler;

	//Udp Socket Handler
	private UDPHandler UHandler;

    //received data from arduino;
    int[] flexData = new int[5];
    float[] ypr = new float[3];

	//data for openCV
	float beforeXPos;
	float beforeYPos;


    private GameObject hand;
	private float speed = 5f;

	//finger
	private GameObject finger_1;
	private GameObject finger_2;
	private GameObject finger_3;

	// thumb variable
	private GameObject thumb_0;
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

	// rotate value
	private int finger_flex;
	private int finger_degree;

	// flex value
	public int thumb_flex;
	public int index_finger_flex;
	public int middle_finger_flex;
	public int ring_finger_flex;
	public int pinky_flex;

	// catch ball
	private GameObject catch_ball_object;
	public bool catch_ball;
	public bool add_score;
	private Animation anim;
	private float timer;
	private int score;
	public int fail_ball_num;

	// baseball UI
	public GameObject baseball1;
	public GameObject baseball2;
	public GameObject baseball3;

	// UI
	public Text scoreText;

	// mouse click point
	private Vector3 mouseWorldPosition;

//-----------------------------------------------------------------start----------------------------------------------------------------------------------
    void Start()
	{
		//get SPHandler
		try{
      		SPHandler = GameObject.Find("SP").GetComponent<SerialPortHandler>();
    	}
    	catch(Exception e){
      		Debug.Log(e);
    	}

		//get UHandler
    	try{
      		UHandler = GameObject.Find("UP").GetComponent<UDPHandler>();
    	}
    	catch(Exception e){
      		Debug.Log(e);
    	}

		// animation component
		anim = this.GetComponent<Animation>();

		// catch ball object
		catch_ball_object = GameObject.Find("Catch_Ball");
		catch_ball_object.gameObject.SetActive(false);

		// timer
		timer = 0.0f;

		// catch ball
		score = 0;
		scoreText.text = string.Format("Score: {0}", score);
		catch_ball = false;
		add_score = false;
		fail_ball_num = 0;

		// read all Children of current object
		Transform[] allChildren = GetComponentsInChildren<Transform>();

		// iterate
		foreach (Transform child in allChildren)
		{
			// find hand object
			if (child.name == "Hand")
				hand = child.gameObject;

			// find thumb object
			if (child.name == "thumb_0")
				thumb_0 = child.gameObject;
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

		// initialization
		mouseWorldPosition = hand.transform.position;

		finger_flex = 180;
		finger_degree = 0;

		thumb_flex = 180;
		index_finger_flex = 180;
		middle_finger_flex = 180;
		ring_finger_flex = 180;
		pinky_flex = 180;

		catch_ball = false;

		// set finger as release
		thumb_0.transform.localEulerAngles = new Vector3(-28.32f, ((-finger_degree - 160) / 5), -25.86f);
		thumb_1.transform.localEulerAngles = new Vector3(2.37f, -0.297f, -finger_degree) * 0.5f;
		thumb_2.transform.localEulerAngles = new Vector3(1.36f, -0.126f, -finger_degree) * 0.3f;

		index_finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		index_finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		index_finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		middle_finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		middle_finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		middle_finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		ring_finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		ring_finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		ring_finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		pinky_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		pinky_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		pinky_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;
        
	}

//--------------------------------------------------------------------update--------------------------------------------------------------------------------
    // Update is called once per frame
	void Update()
	{
        if(SPHandler.ReceiveArduinoData(ref flexData,ref ypr)){
            RotateFinger(flexData);
            hand.transform.rotation = Quaternion.Euler(ypr[2],ypr[1],ypr[0]);
        }

		if(UHandler.newData){
			MoveHand();
		}


		// change finger (thumb)
		if (Input.GetKeyDown(KeyCode.Alpha1))
		{
			finger_flex = thumb_flex;
			finger_1 = thumb_0;
			finger_2 = thumb_1;
			finger_3 = thumb_2;
		}
		// change finger (index finger)
		if (Input.GetKeyDown(KeyCode.Alpha2))
		{
			finger_flex = index_finger_flex;
			finger_1 = index_finger_1;
			finger_2 = index_finger_2;
			finger_3 = index_finger_3;
		}
		// change finger (middle finger)
		if (Input.GetKeyDown(KeyCode.Alpha3))
		{
			finger_flex = middle_finger_flex;
			finger_1 = middle_finger_1;
			finger_2 = middle_finger_2;
			finger_3 = middle_finger_3;
		}
		// change finger (ring finger)
		if (Input.GetKeyDown(KeyCode.Alpha4))
		{
			finger_flex = ring_finger_flex;
			finger_1 = ring_finger_1;
			finger_2 = ring_finger_2;
			finger_3 = ring_finger_3;
		}
		// change finger (pinky)
		if (Input.GetKeyDown(KeyCode.Alpha5))
		{
			finger_flex = pinky_flex;
			finger_1 = pinky_1;
			finger_2 = pinky_2;
			finger_3 = pinky_3;
		}

		//Vector3 angle = Quaternion.Euler(0f, 3f, 0f);
		if (Input.GetKey(KeyCode.UpArrow) && finger_1 != thumb_1)
		{
			// increase value
			finger_flex += 1;
			if (finger_flex > 180)
			{
				finger_flex = 180;
			}

			// set finger degree using flex value
			finger_degree = finger_flex - 180;

			if (finger_degree > 0)
			{
				finger_degree = 0;
			}

			// rotate finger
			finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
			finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
			finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

			// update finger flex value
			if (finger_1 == index_finger_1)
			{
				index_finger_flex = finger_flex;
			}
			if (finger_1 == middle_finger_1)
			{
				middle_finger_flex = finger_flex;
			}
			if (finger_1 == ring_finger_1)
			{
				ring_finger_flex = finger_flex;
			}
			if (finger_1 == pinky_1)
			{
				pinky_flex = finger_flex;
			}

			// print rotation value
			Debug.Log(finger_flex);
			Debug.Log("--------------------------------------------------");
		}
		if (Input.GetKey(KeyCode.DownArrow) && finger_1 != thumb_1)
		{
			// decrease value
			finger_flex -= 1;

			if (finger_flex < 50)
			{
				finger_flex = 50;
			}

			// set finger degree using flex value
			finger_degree = finger_flex - 180;

			if (finger_degree < -130)
			{
				finger_degree = -130;
			}

			// rotate finger
			finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
			finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
			finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

			// update finger flex value
			if (finger_1 == index_finger_1)
			{
				index_finger_flex = finger_flex;
			}
			if (finger_1 == middle_finger_1)
			{
				middle_finger_flex = finger_flex;
			}
			if (finger_1 == ring_finger_1)
			{
				ring_finger_flex = finger_flex;
			}
			if (finger_1 == pinky_1)
			{
				pinky_flex = finger_flex;
			}

			// print rotation value
			Debug.Log(finger_flex);
			Debug.Log("--------------------------------------------------");
		}

		if (Input.GetKey(KeyCode.UpArrow) && finger_1 == thumb_0)
		{
			// increase value
			finger_flex += 1;

			if (finger_flex > 180)
			{
				finger_flex = 180;
			}

			// set finger degree using flex value
			finger_degree = finger_flex - 180;

			if (finger_degree > 0)
			{
				finger_degree = 0;
			}

			finger_1.transform.localEulerAngles = new Vector3(-28.32f, ((-finger_degree - 160) / 5), -25.86f);
			finger_2.transform.localEulerAngles = new Vector3(2.37f, -0.297f, -finger_degree) * 0.5f;
			finger_3.transform.localEulerAngles = new Vector3(1.36f, -0.126f, -finger_degree) * 0.3f;

			thumb_flex = finger_flex;

			// print rotation value
			Debug.Log(finger_flex);
			Debug.Log("--------------------------------------------------");
		}
		if (Input.GetKey(KeyCode.DownArrow) && finger_1 == thumb_0)
		{
			// decrease value
			finger_flex -= 1;

			if (finger_flex < 50)
			{
				finger_flex = 50;
			}

			// set finger degree using flex value
			finger_degree = finger_flex - 180;

			if (finger_degree < -130)
			{
				finger_degree = -130;
			}

			// rotate finger (change x value)
			finger_1.transform.localEulerAngles = new Vector3(-28.32f, ((-finger_degree - 160) / 5), -25.86f);
			finger_2.transform.localEulerAngles = new Vector3(2.37f, -0.297f, -finger_degree) * 0.5f;
			finger_3.transform.localEulerAngles = new Vector3(1.36f, -0.126f, -finger_degree) * 0.3f;

			thumb_flex = finger_flex;

			// print rotation value
			Debug.Log(finger_flex);
			Debug.Log("--------------------------------------------------");
		}

		// when mouse button down
		if (Input.GetMouseButtonUp(0))
		{
			// change hand's world coordinate to screen coordinate (to get z(depth) value)
			Vector3 handScreenPosition = Camera.main.WorldToScreenPoint(hand.transform.position);

			// change mouse's screen coordinate to world coordinate
			mouseWorldPosition = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, handScreenPosition.z));
			print(Input.mousePosition.x);
			print(Input.mousePosition.y);
			//mouseWorldPosition.y = mouseWorldPosition.y - 1.15f;
			//mouseWorldPosition.y = mouseWorldPosition.y;
		}

		// move hand
		if (hand.transform.position != mouseWorldPosition)
		{
			hand.transform.position = Vector3.MoveTowards(hand.transform.position, mouseWorldPosition, speed * Time.deltaTime);
		}

		// rotate hand
		// Roll (x)
		if (Input.GetKey(KeyCode.R))
		{
			speed = 5f;
			hand.transform.Rotate(new Vector3(speed, 0, 0), Space.World);
		}
		// Pitch (y)
		if (Input.GetKey(KeyCode.P))
		{
			speed = 5f;
			hand.transform.Rotate(new Vector3(0, speed, 0), Space.World);
		}
		// Yaw (z)
		if (Input.GetKey(KeyCode.Y))
		{
			speed = 5f;
			hand.transform.Rotate(new Vector3(0, 0, speed), Space.World);
		}

		// catch ball
		if (catch_ball)
		{
			// create catch ball and add score
			if (add_score)
			{
				// create catch ball
				catch_ball_object.transform.localPosition = new Vector3(0, -0.04f, -0.115f);//new Vector3(hand.transform.position.x, hand.transform.position.y + 1.2f, hand.transform.position.z - 0.4f) - hand.transform.up;
				catch_ball_object.gameObject.SetActive(true);

				// play animation
				anim.Play("hold_ball");

				// add score and update UI
				score = score + 10;
				scoreText.text = string.Format("Score: {0}", score);
				add_score = false;
			}

			timer += Time.deltaTime;

			// when animation end
			if (timer > 1.7f)
			{
				catch_ball = false;
				// catch_ball_object.gameObject.SetActive(false);
				timer = 0.0f;

				// set finger as flex value
				thumb_0.transform.localEulerAngles = new Vector3(-28.32f, ((-(thumb_flex - 180) - 160) / 5), -25.86f);
				thumb_1.transform.localEulerAngles = new Vector3(2.37f, -0.297f, -(thumb_flex - 180)) * 0.5f;
				thumb_2.transform.localEulerAngles = new Vector3(1.36f, -0.126f, -(thumb_flex - 180)) * 0.3f;

				index_finger_1.transform.localEulerAngles = new Vector3((index_finger_flex - 180), 0, 0) * 0.5f;
				index_finger_2.transform.localEulerAngles = new Vector3((index_finger_flex - 180), 0, 0) * 0.8f;
				index_finger_3.transform.localEulerAngles = new Vector3((index_finger_flex - 180), 0, 0) * 0.3f;

				middle_finger_1.transform.localEulerAngles = new Vector3((middle_finger_flex - 180), 0, 0) * 0.5f;
				middle_finger_2.transform.localEulerAngles = new Vector3((middle_finger_flex - 180), 0, 0) * 0.8f;
				middle_finger_3.transform.localEulerAngles = new Vector3((middle_finger_flex - 180), 0, 0) * 0.3f;

				ring_finger_1.transform.localEulerAngles = new Vector3((ring_finger_flex - 180), 0, 0) * 0.5f;
				ring_finger_2.transform.localEulerAngles = new Vector3((ring_finger_flex - 180), 0, 0) * 0.8f;
				ring_finger_3.transform.localEulerAngles = new Vector3((ring_finger_flex - 180), 0, 0) * 0.3f;

				pinky_1.transform.localEulerAngles = new Vector3((pinky_flex - 180), 0, 0) * 0.5f;
				pinky_2.transform.localEulerAngles = new Vector3((pinky_flex - 180), 0, 0) * 0.8f;
				pinky_3.transform.localEulerAngles = new Vector3((pinky_flex - 180), 0, 0) * 0.3f;

			}
		}

		// exit game
		if (fail_ball_num > 2)
		{
			SceneManager.LoadScene("GameOverScene");
		}
	}

	public void GripHand()
	{
		// set finger as grip & set finger's flex value

		// thumb
		finger_degree = -50;

		thumb_0.transform.localEulerAngles = new Vector3(-28.32f, ((-finger_degree - 160) / 5), -25.86f);
		thumb_1.transform.localEulerAngles = new Vector3(2.37f, -0.297f, -finger_degree) * 0.5f;
		thumb_2.transform.localEulerAngles = new Vector3(1.36f, -0.126f, -finger_degree) * 0.3f;

		thumb_flex = finger_degree + 180;

		// index finger
		finger_degree = -70;

		index_finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		index_finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		index_finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		index_finger_flex = finger_degree + 180;

		// middle finger & ring finger
		finger_degree = -65;

		middle_finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		middle_finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		middle_finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		middle_finger_flex = finger_degree + 180;

		ring_finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		ring_finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		ring_finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		ring_finger_flex = finger_degree + 180;

		finger_degree = -55;

		// pinky
		pinky_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		pinky_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		pinky_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		pinky_flex = finger_degree + 180;
	}

	public void ReleaseHand()
	{
		finger_degree = 0;

		// set finger as release
		thumb_0.transform.localEulerAngles = new Vector3(-28.32f, ((-finger_degree - 160) / 5), -25.86f);
		thumb_1.transform.localEulerAngles = new Vector3(2.37f, -0.297f, -finger_degree) * 0.5f;
		thumb_2.transform.localEulerAngles = new Vector3(1.36f, -0.126f, -finger_degree) * 0.3f;

		index_finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		index_finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		index_finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		middle_finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		middle_finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		middle_finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		ring_finger_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		ring_finger_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		ring_finger_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		pinky_1.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.5f;
		pinky_2.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.8f;
		pinky_3.transform.localEulerAngles = new Vector3(finger_degree, 0, 0) * 0.3f;

		//set finger's flex value
		thumb_flex = finger_degree + 180;
		index_finger_flex = finger_degree + 180;
		middle_finger_flex = finger_degree + 180;
		ring_finger_flex = finger_degree + 180;
		pinky_flex = finger_degree + 180;
	}

	public void ResetHand()
	{
		hand.transform.localEulerAngles = new Vector3(85, 0, 0);
	}

    void RotateFinger(int[] intDataArr){

		int[] rotate_degree = new int[5];

		for(int i=0;i<5;i++){
			rotate_degree[i] = 180 - intDataArr[i];
		}

        thumb_0.transform.localEulerAngles = new Vector3(-28.32f, ((-rotate_degree[0] - 160) / 5), -25.86f);
		thumb_1.transform.localEulerAngles = new Vector3(2.37f, -0.297f, -rotate_degree[0]) * 0.5f;
		thumb_2.transform.localEulerAngles = new Vector3(1.36f, -0.126f, -rotate_degree[0]) * 0.3f;

		index_finger_1.transform.localEulerAngles = new Vector3(rotate_degree[1], 0, 0) * 0.5f;
		index_finger_2.transform.localEulerAngles = new Vector3(rotate_degree[1], 0, 0) * 0.8f;
		index_finger_3.transform.localEulerAngles = new Vector3(rotate_degree[1], 0, 0) * 0.3f;
		
		middle_finger_1.transform.localEulerAngles = new Vector3(rotate_degree[2], 0, 0) * 0.5f;
		middle_finger_2.transform.localEulerAngles = new Vector3(rotate_degree[2], 0, 0) * 0.8f;
		middle_finger_3.transform.localEulerAngles = new Vector3(rotate_degree[2], 0, 0) * 0.3f;
		
		ring_finger_1.transform.localEulerAngles = new Vector3(rotate_degree[3], 0, 0) * 0.5f;
		ring_finger_2.transform.localEulerAngles = new Vector3(rotate_degree[3], 0, 0) * 0.8f;
		ring_finger_3.transform.localEulerAngles = new Vector3(rotate_degree[3], 0, 0) * 0.3f;
		
		pinky_1.transform.localEulerAngles = new Vector3(rotate_degree[4], 0, 0) * 0.5f;
		pinky_2.transform.localEulerAngles = new Vector3(rotate_degree[4], 0, 0) * 0.8f;
		pinky_3.transform.localEulerAngles = new Vector3(rotate_degree[4], 0, 0) * 0.3f;
	}

	void MoveHand(){
		string text = UHandler.text;

		Vector3 handScreenPosition = Camera.main.WorldToScreenPoint(hand.transform.position);

    	int index1 = text.IndexOf(',');
    	int index2 = text.Length - index1 - 1;
    	String string_xpos = text.Substring(0,index1);
    	String string_ypos = text.Substring(index1+1,index2);

    	float xPos = float.Parse(string_xpos);
    	float yPos = float.Parse(string_ypos);

    	//print(xPos);
    	//print(yPos);

    	//filter1
    	xPos = (float)(xPos * 0.8 + beforeXPos * 0.2);
    	yPos = (float)(yPos * 0.8 + beforeYPos * 0.2);



		//filter2
    	if( ((beforeXPos - xPos) * (beforeXPos - xPos) > 10) || ((beforeYPos - yPos) * (beforeYPos - yPos) > 10) )
    	{

      		mouseWorldPosition = Camera.main.ScreenToWorldPoint(new Vector3(xPos, yPos, handScreenPosition.z));

      		Debug.Log(mouseWorldPosition);
      		hand.transform.position = new Vector3(mouseWorldPosition.x,mouseWorldPosition.y,mouseWorldPosition.z);

      		beforeXPos = xPos;
      		beforeYPos = yPos;
    	}
		
		UHandler.newData = false;
	}
}