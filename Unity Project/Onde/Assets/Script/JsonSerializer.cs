using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.IO;
using UnityEngine.WSA;
using UnityEditor;
using System.Transactions;

public class ChangeMatEventArgs : EventArgs
{
    public Material mat;

    public ChangeMatEventArgs(Material mat)
    {
        this.mat = mat;
    }
 }

public delegate void ChangeMatEventHandler(System.Object sender, ChangeMatEventArgs e);

[Serializable]
public class JsonSerializer  : MonoBehaviour
{
    public event ChangeMatEventHandler ChangeMat;

    // WARNING MUST BE PUBLIC TO SERIALIZED
    [SerializeField]
    public Vector4[] m_sources ;
    

    static string Folder = "Config";

    static string sMatFolder = "Material";

    public static string sRelativeAssetMatFolder()
    {
        return Path.Combine("Assets", sMatFolder);
    }

    public static string sCompleteMatFolder()
    {
        return Path.Combine(UnityEngine.Application.dataPath, sMatFolder);
    }

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


    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void LoadMat(GameObject obj, string matName)
    {
        var matfilename = Path.Combine(sRelativeAssetMatFolder(), matName + ".mat");
        var mat = (Material)AssetDatabase.LoadAssetAtPath<Material>(matfilename);
        var newMat =  new Material(mat);
        newMat.name = "current_"+newMat.name;
        if (mat == null)
        {
            Debug.LogError("LoadMat failed : " + matfilename);
            return;
        }

        if (UnityEngine.Application.isPlaying)
        {
            obj.gameObject.GetComponent<MeshRenderer>().material = newMat;
            var e = new ChangeMatEventArgs(newMat);
            OnChangeMat(e);
        }
        else
        {
            obj.gameObject.GetComponent<MeshRenderer>().sharedMaterial = newMat;
        }

    }



    public void SaveMat(GameObject obj, string matName)
    {
        var matfilename = Path.Combine(sRelativeAssetMatFolder(), matName + ".mat");

        Material mat;

        if(UnityEngine.Application.isPlaying)
        {
            mat = obj.GetComponent<MeshRenderer>().material;
        }else
        {
            mat = obj.GetComponent<MeshRenderer>().sharedMaterial;
        }

        if (mat == null)
        {
            Debug.LogError("SaveMat failed : " + matName);
            return;
        }
        Material newMat = new Material(mat);

        newMat.name = matName;
        if (AssetDatabase.Contains(newMat))
        {
            Debug.LogWarning("Material already exist, will be overwritten !!");

        }

        AssetDatabase.CreateAsset(newMat,matfilename);

        if (AssetDatabase.Contains(newMat))
            Debug.Log("Material asset created at "+matfilename);
        mat.name = "current";


    }



    protected virtual void OnChangeMat(ChangeMatEventArgs mat)
    {
        if(ChangeMat != null)
        {
            ChangeMat(this,mat);
        }
    }


}


