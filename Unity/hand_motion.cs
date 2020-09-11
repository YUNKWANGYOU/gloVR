// Hand asset을 움직이게 하기 위한 스크립트
// 손가락 각도 조절: 화살표 방향키 위(펴기), 아래(굽히기)
// 손가락 변경: 1~5 (엄지~새끼)
// 손 이동: 마우스 클릭한 좌표로 이동
// 손 회전: Y(yaw), P(pitch), R(roll)
// Grip과 Release, Reset 기능 있음 (Reset은 회전 각 초기화)
// 값이 50~180이 들어온다는 가정 하에 만듬. (혹시 입력 값이 바뀐다면 스크립트 수정해야함)

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class hand_motion : MonoBehaviour
{
	// hand
	private GameObject hand;

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
	private int thumb_flex;
	private int index_finger_flex;
	private int middle_finger_flex;
	private int ring_finger_flex;
	private int pinky_flex;

	// mouse click point
	private Vector3 mouseWorldPosition;

	// Start is called before the first frame update
	void Start()
	{
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

		//defaultHandRotation = hand.transform.rotation;
	}

	// Update is called once per frame
	void Update()
	{
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
			finger_flex += 1;
			if (finger_flex > 180)
			{
				finger_flex = 180;
			}

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
			Debug.Log(finger_degree);
			Debug.Log("--------------------------------------------------");
		}
		if (Input.GetKey(KeyCode.DownArrow) && finger_1 != thumb_1)
		{
			finger_flex -= 1;
			if (finger_flex < 50)
			{
				finger_flex = 50;
			}

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
			Debug.Log(finger_degree);
			Debug.Log("--------------------------------------------------");
		}

		if (Input.GetKey(KeyCode.UpArrow) && finger_1 == thumb_0)
		{
			finger_flex += 1;
			if (finger_flex > 180)
			{
				finger_flex = 180;
			}

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
			Debug.Log(finger_degree);
			Debug.Log("--------------------------------------------------");
		}
		if (Input.GetKey(KeyCode.DownArrow) && finger_1 == thumb_0)
		{
			finger_flex -= 1;
			if (finger_flex < 50)
			{
				finger_flex = 50;
			}

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
			Debug.Log(finger_degree);
			Debug.Log("--------------------------------------------------");
		}

		// when mouse button down
		if (Input.GetMouseButtonUp(0))
		{
			// change hand's world coordinate to screen coordinate (to get z(depth) value)
			Vector3 handScreenPosition = Camera.main.WorldToScreenPoint(hand.transform.position);

			// change mouse's screen coordinate to world coordinate
			mouseWorldPosition = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, handScreenPosition.z));
			mouseWorldPosition.y = mouseWorldPosition.y - 1.15f;

			Debug.Log(mouseWorldPosition);
		}

		// move hand
		if (hand.transform.position != mouseWorldPosition)
		{
			float speed = 2f;
			hand.transform.position = Vector3.MoveTowards(hand.transform.position, mouseWorldPosition, speed * Time.deltaTime);
		}

		// rotate hand
		// Roll (x)
		if (Input.GetKey(KeyCode.R))
		{
			float speed = 5f;
			hand.transform.Rotate(new Vector3(speed, 0, 0), Space.World);
		}
		// Pitch (y)
		if (Input.GetKey(KeyCode.P))
		{
			float speed = 5f;
			hand.transform.Rotate(new Vector3(0, speed, 0), Space.World);
		}
		// Yaw (z)
		if (Input.GetKey(KeyCode.Y))
		{
			float speed = 5f;
			hand.transform.Rotate(new Vector3(0, 0, speed), Space.World);
		}
	}

	public void GripHand()
	{
		finger_degree = -130;

		// set finger as grip
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
}
