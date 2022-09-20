using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.IO;
using UnityEngine.WSA;

[Serializable]
public class JsonSerializer 
{
    // WARNING MUST BE PUBLIC TO SERIALIZED
    [SerializeField]
    public Vector4[] m_sources ;
    

    static string Folder = "Config";
    [Serializable]
    public class Vec4
    {

        Vec4(UnityEngine.Vector4 vector4)
        {
            x = vector4.x;
            y = vector4.y;
            z = vector4.z;
            w = vector4.w;
        }

        float x;
        float y;
        float z;
        float w;
    }


    public void SaveConfig(OndeSphereShaderInterface shader_interface, string nameConfig)
    {
        m_sources = shader_interface.Sources;
        var str = JsonUtility.ToJson(this,true);
        var filename = Path.Combine(sCompleteFolder(), nameConfig+".json");
        File.WriteAllText(filename, str);
        Debug.Log("Save File in " + filename);

    }


    public void LoadConfig(OndeSphereShaderInterface shader_interface, string nameConfig)
    {
        var filename = Path.Combine(sCompleteFolder(), nameConfig + ".json");
        if (!File.Exists(filename))
        {
            Debug.LogWarning("loadingFile : missing file " + filename);
            return;
        }

        var str = File.ReadAllText(filename);
        

        var jsonSerialized = new JsonSerializer();
        jsonSerialized = JsonUtility.FromJson<JsonSerializer>(str);
       
        Debug.Log("Loaded File " + filename);

        shader_interface.Sources = jsonSerialized.m_sources;
    

    }

    static public string sCompleteFolder()
    {
        return Path.Combine(UnityEngine.Application.dataPath, Folder);
    }

    static public List<string> GetFolderContent()
    {
        var list_string = Directory.GetFiles(sCompleteFolder());

        var ret = new List<string>();

        foreach(var file in list_string)
        {
            if(Path.GetExtension(file)==".json")
            {
                ret.Add(Path.GetFileNameWithoutExtension(file));
            }
        }

        return ret;
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
