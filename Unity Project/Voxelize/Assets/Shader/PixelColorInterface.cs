using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PixelColorInterface : MonoBehaviour
{
    [SerializeField] Material _mat;

    // Start is called before the first frame update
    void Start()
    {
        Shader s = Resources.Load("Shader/PixelColorShader") as Shader;
        Camera.main.SetReplacementShader(s, "Pixelise");
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
