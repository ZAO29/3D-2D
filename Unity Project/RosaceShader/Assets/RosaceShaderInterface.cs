using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RosaceShaderInterface : MonoBehaviour
{

    [SerializeField] Material _mat;
    float _multTime = 1;
    [SerializeField]
    int _NbPoint = 4;

    //ShaderPropertyID
    int _scriptTimeID;
    int _nbPointID;

    // Use this for initialization
    void Start()
    {
        _scriptTimeID = Shader.PropertyToID("_ScriptTime");
        _nbPointID = Shader.PropertyToID("_NbPoint");
    }

    // Update is called once per frame
    void Update()
    {
        UpdateMultTimeOnTouch(2.0f, KeyCode.RightArrow);
        UpdateMultTimeOnTouch(0.5f, KeyCode.LeftArrow);

        if (Input.GetMouseButtonDown(0))
        {
            Vector3 pos = Camera.main.ScreenToViewportPoint(Input.mousePosition);
            if (pos.x > 0.5)
            {
                if(pos.y>0.5)
                {
                    _NbPoint *= 2;
                }else
                {
                    _NbPoint /= 2;
                }
                    
            }
            else
            {
                if (pos.y > 0.5)
                {
                    _multTime *= 2;
                }
                else
                {
                    _multTime /= 2;
                }
            }

        }
        if (_NbPoint <= 4)
        {
            _NbPoint = 4;
        }
    }
    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        _mat.SetFloat(_scriptTimeID, Time.realtimeSinceStartup*_multTime);
        _mat.SetInt(_nbPointID, _NbPoint);
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
