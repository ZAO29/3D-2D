using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NormalShaderInterface : MonoBehaviour
{
    [SerializeField] private Material _Material;
    [SerializeField] private float _Freq;
    [SerializeField] private float _Amplitude;
    // Start is called before the first frame update
    void Start()
    {
        _Material = GetComponent<Renderer>().material;
        _Material.SetFloat("_Frequency", _Freq);
        _Material.SetFloat("_Amplitude", _Amplitude);

    }

    // Update is called once per frame
    void Update()
    {
        _Material.SetFloat("_Frequency", _Freq);
        _Material.SetFloat("_Amplitude", _Amplitude);     
    }
}
