using System;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using TMPro;
using UnityEngine.UIElements;
using System.Globalization;
using UnityEngine.UI;

public class OndeSphereShaderInterface : MonoBehaviour {


    [SerializeField]
    TMP_InputField spatialfreqText;

    [SerializeField]
    UnityEngine.UI.Button ValidateBtn;


    Material _mat;
    int _nbSource = 2;

    [SerializeField] Vector4[] _sources;

    [SerializeField] float freqSpatial = 0.0f;
    [SerializeField] float interpolateTime = 3.0f;
    float oldFreqSpatial = 0.0f;


    //ShaderPropertyID
    int _scriptTimeID;
    int _nbSourceID;
    int _sourcesID;


    public Vector4[] Sources { get => _sources; set => _sources = value; }

    // Use this for initialization
    void Awake()
    {
        var serializer = FindObjectOfType<JsonSerializer>();
        Debug.Assert(serializer != null);
        Debug.Assert(ValidateBtn != null);
        Debug.Assert(spatialfreqText != null);

        serializer.ChangeMat += OnChangeMat;
        InitMat();
        ValidateBtn.onClick.AddListener(() =>
        {
            freqSpatial = float.Parse(spatialfreqText.text, CultureInfo.InvariantCulture);
            Debug.Log("new freq Spatial " + freqSpatial);
        });
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

        freqSpatial = _mat.GetFloat("_FreqSpatial");
        oldFreqSpatial = freqSpatial;
        freqSpatial = _mat.GetFloat("_FreqSpatial");
        spatialfreqText.text = freqSpatial.ToString();

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

        if(freqSpatial != oldFreqSpatial)
        {
            StartCoroutine(FreqSpatialInterpolate(oldFreqSpatial, freqSpatial, interpolateTime));
            oldFreqSpatial = freqSpatial;

        }
   
    }

    IEnumerator FreqSpatialInterpolate(float formerValue, float newValue, float duration)
    {
        float startTime = Time.time;
        float elapsedTime = Time.time - startTime;


        while (elapsedTime < duration)
        {
            elapsedTime = Time.time - startTime;
            // calculate the current value based on the elapsed time and duration
            float progress = elapsedTime / duration;
            float currentVal = Mathf.SmoothStep(formerValue, newValue, progress);


            _mat.SetFloat("_FreqSpatial",currentVal);
            // wait for a short time before calculating the next value
            yield return new WaitForSeconds(0.01f);



        }

        _mat.SetFloat("_FreqSpatial", newValue);

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
