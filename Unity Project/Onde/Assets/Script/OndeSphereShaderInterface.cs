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

    public Vector4[] Sources { get => _sources; set => _sources = value; }

    // Use this for initialization
    void Start()
    {
        _scriptTimeID = Shader.PropertyToID("_ScriptTime");
        _nbSourceID = Shader.PropertyToID("_nbSource");
        _sourcesID = Shader.PropertyToID("_Sources");
        _mat = GetComponent<MeshRenderer>().material;
        Debug.Assert(_mat != null);
        Debug.Log("ID " + _sourcesID + " " + "_scriptTimeID " + _scriptTimeID);

        // Skybox/Cubemap (Shader)
        var tex = RenderSettings.skybox.GetTexture("_Tex");
        _mat.SetTexture("_SkyTex3D", tex);

    }

    // Update is called once per frame
    void Update () {
        _mat.SetFloat(_scriptTimeID, Time.realtimeSinceStartup);

        Vector4 central = Vector4.Normalize(new Vector4(1, 1, 1, 0));

        Vector4[] listVec = new Vector4[_sources.Length];

        float coeff = (1.0f + Mathf.Cos(Time.realtimeSinceStartup / 10.0f)) / 2.0f;

        for (int i=0;i<_sources.Length;i++)
        {
            listVec[i] = Vector4.Normalize(Vector4.Lerp(central, _sources[i], coeff));
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
