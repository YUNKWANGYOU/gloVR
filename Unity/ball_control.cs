using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ball_control : MonoBehaviour
{
	// hand
	private GameObject hand;

	// target vector
	private Vector3 targetPos;
	private Vector3 moveVector;

	// Start is called before the first frame update
	void Start()
    {
		// find Hand object
		hand = GameObject.Find("Hand");

		// put ball at start point
		targetPos = new Vector3(0, 0.5f, -11f);
		this.transform.position = targetPos;
		moveVector = new Vector3(0, 0, 0);
	}

    // Update is called once per frame
    void Update()
    {
		if (Input.GetKeyUp(KeyCode.S))
		{
			// put ball at start point
			this.transform.position = new Vector3(0, 0.5f, -11f);

			// set target point
			float targetX = Random.Range(hand.transform.position.x - 1, hand.transform.position.x + 1);
			float targetY = Random.Range(hand.transform.position.y - 1, hand.transform.position.y + 1) + 0.95f;
			if (targetY < 0.5f)
			{
				targetY = 0.5f;
			}
			float targetZ = hand.transform.position.z - 0.46f;

			targetPos = new Vector3(targetX, targetY, targetZ);
			moveVector = targetPos - this.transform.position;

			Debug.Log(targetPos);
		}
		
		if (this.transform.position != targetPos)
		{
			float speed = 0.5f;
			this.transform.Translate(moveVector * Time.deltaTime * speed, Space.World);
			this.transform.Rotate(moveVector);
		}
	}
}
