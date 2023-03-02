using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OndePlanShaderInterface : MonoBehaviour {

    [SerializeField] Material _mat;
    [SerializeField] int _nbSource = 2;

    //ShaderPropertyID
    int _scriptTimeID;
    int _nbSourceID;

    // Use this for initialization
    void Start()
    {
        _scriptTimeID = Shader.PropertyToID("_ScriptTime");
        _nbSourceID = Shader.PropertyToID("_nbSource");
    }

    // Update is called once per frame
    void Update () {
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



    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        _mat.SetFloat(_scriptTimeID, Time.realtimeSinceStartup);
        _mat.SetInt(_nbSourceID, _nbSource);
        Graphics.Blit(source, destination, _mat);


    }
}
