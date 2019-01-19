using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OndeShaderInterface : MonoBehaviour {

    [SerializeField] Material _mat;


    //ShaderPropertyID
    int _scriptTimeID;

    // Use this for initialization
    void Start()
    {
        _scriptTimeID = Shader.PropertyToID("_ScriptTime");
    }

    // Update is called once per frame
    void Update () {
		
	}

    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        _mat.SetFloat(_scriptTimeID, Time.realtimeSinceStartup);
        Graphics.Blit(source, destination, _mat);


    }
}
