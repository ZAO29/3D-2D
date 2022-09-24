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
    JsonSerializer m_serializer = new JsonSerializer();
    List<string> m_savedFiles = new List<string>();
    List<string> m_savedMat = new List<string>();
    int m_selectedIdFiles = 0;
    int m_selectedIdMat = 0;

    


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
            m_serializer.SaveConfig(obj, m_filename);
            Refresh();
        }

       
        m_selectedIdFiles = EditorGUILayout.Popup(m_selectedIdFiles, m_savedFiles.ToArray());


        if (GUILayout.Button("LoadConfig"))
        {
            m_serializer.LoadConfig(obj, m_savedFiles[m_selectedIdFiles]);
        }


        m_selectedIdMat = EditorGUILayout.Popup(m_selectedIdMat, m_savedMat.ToArray());



        if (GUILayout.Button("LoadMat"))
        {
            m_serializer.LoadMat(obj.gameObject, m_savedMat[m_selectedIdMat]);
        }

        if (GUILayout.Button("SavedMat"))
        {
            m_serializer.SaveMat(obj.gameObject, m_filename);
            Refresh();
        }


        if (GUILayout.Button("Refresh"))
        {
            Refresh();
        }
    }
}
