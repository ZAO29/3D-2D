using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OndeSphereShaderInterface : MonoBehaviour {





    Material _mat;
    int _nbSource = 2;

    [SerializeField] Vector4[] _sources;

    //ShaderPropertyID
    int _scriptTimeID;
    int _nbSourceID;
    int _sourcesID;
    bool _firstUpdate = true;

    public Vector4[] Sources { get => _sources; set => _sources = value; }

    // Use this for initialization
    void Start()
    {
        var serializer = FindObjectOfType<JsonSerializer>();
        Debug.Assert(serializer != null);
        serializer.ChangeMat += OnChangeMat;
        InitMat();
    }

    void OnChangeMat(System.Object sender, ChangeMatEventArgs e)
    {
        InitMat();
    }

    void InitMat()
    {
        _scriptTimeID = Shader.PropertyToID("_ScriptTime");
        _nbSourceID = Shader.PropertyToID("_nbSource");
        _sourcesID = Shader.PropertyToID("_Sources");
        _mat = GetComponent<MeshRenderer>().material;
        Debug.Assert(_mat != null);
        // Skybox/Cubemap (Shader)
        var tex = RenderSettings.skybox.GetTexture("_Tex");
        _mat.SetTexture("_SkyTex3D", tex);
    }

    // Update is called once per frame
    void Update () {

        _mat.SetFloat(_scriptTimeID, Time.realtimeSinceStartup);

        Vector4 central = Vector4.Normalize(new Vector4(0, 1, 0, 0));

        Vector4[] listVec = new Vector4[_sources.Length];

        float coeff = (1.0f + Mathf.Cos(4.0f*Time.realtimeSinceStartup / 10.0f)) / 2.0f;

        for (int i=0;i<_sources.Length;i++)
        {
            listVec[i] = Vector4.Normalize(Vector4.Lerp( _sources[i],central, coeff));
        }

        _mat.SetInt(_nbSourceID, _sources.Length);
        _mat.SetVectorArray(_sourcesID, listVec);
   
    }
    void UpdateNbSourceOnTouch(int addNumber, KeyCode key)
    {
        if (Input.GetKeyDown(key))
        {
            _nbSource += addNumber;
        }

        if(_nbSource <= 0)
        {
            _nbSource = 1;
        }
    }
}
