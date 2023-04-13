using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OscillatorWidth : MonoBehaviour
{
    // Start is called before the first frame update
    [SerializeField] Material material;

    [SerializeField] float freq;

    [SerializeField] float minVal;

    [SerializeField] float maxVal;

    void Start()
    {
        Debug.Assert(material != null);    
    }

    // Update is called once per frame
    void Update()
    {
        float interpFactor = (1 + Mathf.Cos(freq * Time.time)) / 2.0f;
        float value = Mathf.Lerp(minVal, maxVal, interpFactor);
        material.SetFloat("_EdgeWidth", value);

    }
}
