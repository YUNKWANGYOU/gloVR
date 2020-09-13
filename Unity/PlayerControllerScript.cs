using UnityEngine;
using System;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

public class PlayerControllerScript: MonoBehaviour
{
  // 1. Declare VariablesThread receiveThread; //1
  Thread receiveThread;
  UdpClient client;
  int port;

  public GameObject Player;
  public GameObject hand;
  bool jump;
  string text;

	// 2. Initialize variables
  void Start () 
  {
    port = 5065;
    jump = false; 

    InitUDP();

    foreach(Transform child in allChildren){
      if (child.name == "Hand")
        hand = child.gameObject;
    }
    
  }
  
	// 3. InitUDP
  private void InitUDP()
  {
    print ("UDP Initialized");

    receiveThread = new Thread (new ThreadStart(ReceiveData));
    receiveThread.IsBackground = true;
    receiveThread.Start();
  }
  
	// 4. Receive Data
  private void ReceiveData()
  {
    client = new UdpClient (port);
    while (true)
    {
      try
      {
        IPEndPoint anyIP = new IPEndPoint(IPAddress.Parse("0.0.0.0"), port);
        byte[] data = client.Receive(ref anyIP);

        text = Encoding.UTF8.GetString(data);
        print(">> " + text);

        jump = true;

      } 
      catch(Exception e)
      {
        print (e.ToString());
      }
    }
  }

	// 5. Make the Player move
  public void MoveHand()
  {
    // change hand's world coordinate to screen coordinate (to get z(depth) value)
			Vector3 handScreenPosition = Camera.main.WorldToScreenPoint(hand.transform.position);
			
			// change mouse's screen coordinate to world coordinate
      float xPos = text.Substring();
      float yPos = text.Substring();

			mouseWorldPosition = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, handScreenPosition.z));

			Debug.Log(mouseWorldPosition);

			//hand.transform.position = Vector3.MoveTowards(hand.transform.position, mouseWorldPosition, 1f * Time.deltaTime);

			//Vector3 moveVector = mouseWorldPosition - hand.transform.position;
			//hand.transform.position = mouseWorldPosition;
			// offset = hand's world coordinate - mouse's world coordinate (vector from mouse to hand)
			//Vector3 offset = hand.transform.position - Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, handScr_coor.z));
		}

		// move hand
		if (hand.transform.position != mouseWorldPosition)
		{
			float speed = 2f;
			hand.transform.position = Vector3.MoveTowards(hand.transform.position, mouseWorldPosition, speed * Time.deltaTime);
		}
  }

	// 6. Check for variable value, and make the Player Jump!
  void Update () 
  {
    if(jump == true)
    {
      MoveHand();
      jump = false;
    }
  }


}


// when mouse button down
		if (Input.GetMouseButtonUp(0))
		{
			
