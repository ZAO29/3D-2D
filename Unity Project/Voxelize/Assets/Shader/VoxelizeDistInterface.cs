using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VoxelizeDistInterface : MonoBehaviour
{
    // Start is called before the first frame update
    private Material _Mat;
    [SerializeField]
    public float _Res = 1;
    [SerializeField]
    public bool _bUseDistanceCam = true;
    void Start()
    {
        _Mat = GetComponent<Renderer>().material;
    }

    // Update is called once per frame
    void Update()
    {
        if (_bUseDistanceCam)
        {
            _Res = (Camera.main.transform.position - transform.position).magnitude;

        }
        else
        {
            _Res = (float)Time.realtimeSinceStartup;
        }


        _Mat.SetVector("_Position", Camera.main.transform.position);



    }
}
