using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static TMPro.SpriteAssetUtilities.TexturePacker_JsonArray;
using MF = UnityEngine.Mathf;

public class OndePlanShaderInterface : MonoBehaviour {

    const int MAX_NB_SOURCE = 10;

    [SerializeField] Material _mat;
    [SerializeField] int _nbSource = 2;
    [SerializeField] GameObject backgroundQuad;
    float[] _sources = new float[2*MAX_NB_SOURCE];

    //ShaderPropertyID
    int _scriptTimeID;
    int _nbSourceID;

  

    // Use this for initialization
    void Start()
    {
        _scriptTimeID = Shader.PropertyToID("_ScriptTime");
        _nbSourceID = Shader.PropertyToID("_nbSource");
       

        backgroundQuad.transform.localScale = new Vector3(Camera.main.aspect, 1, 1);
    }

    // Update is called once per frame
    void Update () {

        UpdateSources();

        _mat.SetFloat(_scriptTimeID, Time.realtimeSinceStartup);
        _mat.SetInt(_nbSourceID, _nbSource);
        _mat.SetFloatArray("_Sources", _sources);

        if (Input.GetMouseButtonDown(0))
        {
            Vector3 pos = Camera.main.ScreenToViewportPoint(Input.mousePosition);
            if(pos.x > 0.5)
            {
                _nbSource++;
            }else
            {
                _nbSource--;
            }
           
        }
         if(_nbSource <=0)
        {
            _nbSource = 1;
        }
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


    void UpdateSources()
    {
        if(_nbSource*2 > _sources.Length)
        {
            Debug.LogWarning("too much sources !!!");
            return;
        }

        var oscillation = MF.Pow(1.0f - MF.Abs(MF.Sin(((float)Time.realtimeSinceStartup) / 15.0f)), 1.5f);
        var ratio = Camera.main.aspect / 2.0f;
        var center = new Vector2(ratio, 0.5f);
        float amplitude = oscillation/2.0f;
        var deltaTheta = 2.0f * MF.PI / ((float) _nbSource);

        for (int i= 0; i < _nbSource; i++)
        {
            var theta = ((float)i) * deltaTheta;
            var currentCenter = center + amplitude * new Vector2(MF.Sin(theta),MF.Cos(theta));

            _sources[i * 2] = currentCenter.x;
            _sources[i * 2 + 1] = currentCenter.y;
        }
    }


    /*
    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        //_mat.SetFloat(_scriptTimeID, Time.realtimeSinceStartup);
        //_mat.SetInt(_nbSourceID, _nbSource);
        //Graphics.Blit(source, destination, _mat);


    }*/
}
