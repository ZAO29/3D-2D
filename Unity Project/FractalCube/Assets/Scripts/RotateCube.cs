using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class RotateCube : MonoBehaviour
{
    // Start is called before the first frame update
    [SerializeField] float speed = 0.01f;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        this.transform.eulerAngles = this.transform.eulerAngles + new Vector3(speed,0,0);
    }
}
