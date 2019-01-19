using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Scrolling : MonoBehaviour {

    [SerializeField] private float speed = 0.5f;
    [SerializeField] private float modSize = 20;
    [SerializeField] private float m_Realspeed;
    Vector2 startPos;

	// Use this for initialization
	void Start () {
        startPos = transform.position;
		
	}
	
	// Update is called once per frame
	void Update () {
        BackGroundHandler parent = transform.GetComponentInParent<BackGroundHandler>();
        m_Realspeed = speed *parent.speed;
        float newPos = Mathf.Repeat(Time.time * m_Realspeed, modSize);
        transform.position = startPos + Vector2.left * newPos;
	}
}
