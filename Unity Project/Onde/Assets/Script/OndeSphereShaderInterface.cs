using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OndeSphereShaderInterface : MonoBehaviour {

    [SerializeField] Material _mat;
    int _nbSource = 2;

    //ShaderPropertyID
    int _scriptTimeID;
    int _nbSourceID;

    // Use this for initialization
    void Start()
    {
        _scriptTimeID = Shader.PropertyToID("_ScriptTime");
        _nbSourceID = Shader.PropertyToID("_nbSource");
        _mat = GetComponent<MeshRenderer>().material;
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

        _mat.SetFloat(_scriptTimeID, Time.realtimeSinceStartup);
        _mat.SetInt(_nbSourceID, _nbSource);
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
