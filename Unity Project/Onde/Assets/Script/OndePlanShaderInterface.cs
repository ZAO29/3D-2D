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
    [SerializeField] Vector4 _RGBShift = new Vector4(0, 0, 0, 1);
    public Vector4 RGBShift { get => _RGBShift; private set => _RGBShift = value; }
    public int NbSource { get => _nbSource; private set => _nbSource = value; }
    public Vector2 Sources(int i)
    {
        return new Vector2(_sources[2 * i], _sources[2 * i + 1]);
    }

    //ShaderPropertyID
    int _scriptTimeID;
    int _nbSourceID;

    public float t { get => Time.realtimeSinceStartup; }
   



    // Use this for initialization
    void Start()
    {
        _scriptTimeID = Shader.PropertyToID("_ScriptTime");
        _nbSourceID = Shader.PropertyToID("_nbSource");
       

        backgroundQuad.transform.localScale = new Vector3(Camera.main.aspect, 1, 1);
    }

    // Update is called once per frame
    void Update () {

       
       
        RGBShift = new Vector4(MF.PI / 2.0f * MF.Cos(t),
                               MF.PI / 4.0f * MF.Cos(t * 2.0f),
                               MF.PI / 4.0f * MF.Cos(t * 4.0f),
                               0.0f);
        UpdateSources();


        _mat.SetVector("_RGBShift", RGBShift);
        _mat.SetFloat(_scriptTimeID, t);
        _mat.SetInt(_nbSourceID, NbSource);
        _mat.SetFloatArray("_Sources", _sources);

       

        if (Input.GetMouseButtonDown(0))
        {
            Vector3 pos = Camera.main.ScreenToViewportPoint(Input.mousePosition);
            if(pos.x > 0.5)
            {
                NbSource++;
            }else
            {
                NbSource--;
            }
           
        }
         if(NbSource <=0)
        {
            NbSource = 1;
        }
    }
    void UpdateNbSourceOnTouch(int addNumber, KeyCode key)
    {
        if (Input.GetKeyDown(key))
        {
            NbSource += addNumber;
        }

        if(NbSource <= 0)
        {
            NbSource = 1;
        }
    }


    void UpdateSources()
    {
        if(NbSource*2 > _sources.Length)
        {
            Debug.LogWarning("too much sources !!!");
            return;
        }

        var oscillation = MF.Pow(1.0f - MF.Abs(MF.Sin(((float)Time.realtimeSinceStartup) / 15.0f)), 1.5f);
        var ratio = Camera.main.aspect / 2.0f;
        var center = new Vector2(ratio, 0.5f);
        float amplitude = oscillation/2.0f;
        var deltaTheta = 2.0f * MF.PI / ((float) NbSource);

        if (NbSource == 1)
        {
            _sources[0] = center.x;
            _sources[1] = center.y;
        }
        else
        {
            for (int i = 0; i < NbSource; i++)
            {
                var theta = ((float)i) * deltaTheta;
                var currentCenter = center + amplitude * new Vector2(MF.Sin(theta), MF.Cos(theta));

                _sources[i * 2] = currentCenter.x;
                _sources[i * 2 + 1] = currentCenter.y;
            }
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
