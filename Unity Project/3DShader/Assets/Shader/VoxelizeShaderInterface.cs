using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VoxelizeShaderInterface : MonoBehaviour
{
    // Start is called before the first frame update
    private Material _Mat;
    [SerializeField]
    public float _Res = 1;
    void Start()
    {
        _Mat = GetComponent<Renderer>().material;
    }

    // Update is called once per frame
    void Update()
    {
        // _Res = (float)System.Math.Log((float)Time.realtimeSinceStartup);
        _Res = (float)Time.realtimeSinceStartup * 0.1f +1.0f;
        _Mat.SetFloat("_SpatialRes", _Res);
    }
}
