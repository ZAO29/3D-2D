﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Move : MonoBehaviour
{
    [SerializeField]
    public float _Speed = 1;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {

        if(Input.GetKey(KeyCode.RightArrow))
        {
            transform.Translate(new Vector3(0,_Speed * Time.deltaTime, 0));
        }

        if (Input.GetKey(KeyCode.LeftArrow))
        {
            transform.Translate(new Vector3(0,-_Speed * Time.deltaTime, 0));
        }


        if (Input.GetKey(KeyCode.UpArrow))
        {
            transform.Translate(new Vector3(_Speed * Time.deltaTime,0, 0));
        }

        if (Input.GetKey(KeyCode.DownArrow))
        {
            transform.Translate(new Vector3(-_Speed * Time.deltaTime,0, 0));
        }

        if (Input.GetKey(KeyCode.A))
        {
            transform.Translate(new Vector3(0,0,_Speed * Time.deltaTime));
        }

        if (Input.GetKey(KeyCode.Z))
        {
            transform.Translate(new Vector3(0,0,-_Speed * Time.deltaTime));
        }

    }
}