using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour {


    [SerializeField] public float m_speedStep = 0.0001f;
    float m_startTime = 0;
    [SerializeField] public float m_updateSpeedTime = 0.01f;
    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {

         if(!UpdateSpeed(m_speedStep, KeyCode.RightArrow))
        {
            UpdateSpeed(-m_speedStep, KeyCode.LeftArrow);
        }



    }

    bool UpdateSpeed(float a_step,KeyCode a_key)
    {
        bool rvalue = Input.GetKey(a_key);

        if (Input.GetKeyDown(a_key))
        {
            m_startTime = Time.time;
        }
        if (rvalue && (Time.time - m_startTime) > m_updateSpeedTime)
        {
            m_startTime = Time.time;

            BackGroundHandler parent = transform.GetComponentInParent<BackGroundHandler>();

            if(parent)
            {
                parent.speed += a_step;
            }
        }

        return rvalue;
        
    }
}
