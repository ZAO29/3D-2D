using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.ComponentModel;
using System.IO;
using UnityEngine.WSA;

public class OndeWindow : EditorWindow
{
   
    string m_filename = "defaultConfig";
    JsonSerializer m_serializer = null;
    List<string> m_savedFiles = new List<string>();
    List<string> m_savedMat = new List<string>();
    int m_selectedIdFiles = 0;
    int m_selectedIdMat = 0;

    public JsonSerializer Serializer
    {
        get
        {
            if (m_serializer == null)
            {
                m_serializer = FindObjectOfType<JsonSerializer>();
            }
            return m_serializer;
        }

        set { m_serializer = value; }
    }

    void Refresh()
    {
        m_savedFiles.Clear();
        m_savedFiles = GetFolderContent(JsonSerializer.sCompleteFolder(),".json");
        m_selectedIdFiles = 0;
        m_savedMat.Clear();
        m_savedMat = GetFolderContent(JsonSerializer.sCompleteMatFolder(), ".mat");
        m_selectedIdMat = 0;
    }

    static List<string> GetFolderContent(string folder,string ext)
    {
        var list_string = Directory.GetFiles(folder);

        var ret = new List<string>();

        foreach (var file in list_string)
        {
            if (Path.GetExtension(file) == ext)
            {
                ret.Add(Path.GetFileNameWithoutExtension(file));
            }
        }

        return ret;
    }

    [MenuItem("Tools/Onde")]
    public static void OpenWindow()
    {
        var w = GetWindow<OndeWindow>();
        w.Refresh();
    }



    private void OnGUI()
    {
        m_filename = GUILayout.TextArea(m_filename);

        var obj = FindObjectOfType<OndeSphereShaderInterface>();

        if (obj == null)
        {
            Debug.LogWarning("Save :missing OndeSphereShaderInterface script");
            return;
        }


        if (GUILayout.Button("SaveConfig"))
        {
            Serializer.SaveConfig(obj, m_filename);
            Refresh();
        }

       
        m_selectedIdFiles = EditorGUILayout.Popup(m_selectedIdFiles, m_savedFiles.ToArray());


        if (GUILayout.Button("LoadConfig"))
        {
            Serializer.LoadConfig(obj, m_savedFiles[m_selectedIdFiles]);
        }


        m_selectedIdMat = EditorGUILayout.Popup(m_selectedIdMat, m_savedMat.ToArray());



        if (GUILayout.Button("LoadMat"))
        {
            Serializer.LoadMat(obj.gameObject, m_savedMat[m_selectedIdMat]);
        }

        if (GUILayout.Button("SavedMat"))
        {
            Serializer.SaveMat(obj.gameObject, m_filename);
            Refresh();
        }


        if (GUILayout.Button("Refresh"))
        {
            Refresh();
        }
    }
}
