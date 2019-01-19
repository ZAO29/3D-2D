using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RosaceShaderInterface : MonoBehaviour
{

    [SerializeField] Material _mat;
    float _multTime = 1;

    //ShaderPropertyID
    int _scriptTimeID;

    // Use this for initialization
    void Start()
    {
        _scriptTimeID = Shader.PropertyToID("_ScriptTime");
    }

    // Update is called once per frame
    void Update()
    {
        UpdateMultTimeOnTouch(2.0f, KeyCode.RightArrow);
        UpdateMultTimeOnTouch(0.5f, KeyCode.LeftArrow);
    }
    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        _mat.SetFloat(_scriptTimeID, Time.realtimeSinceStartup*_multTime);
        Graphics.Blit(source, destination, _mat);


    }

    void UpdateMultTimeOnTouch(float mult, KeyCode key)
    {
        if(Input.GetKeyDown(key))
        {
            _multTime *= mult;
        }
    }
}
