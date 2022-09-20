using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.ComponentModel;

public class OndeWindow : EditorWindow
{
   
    string m_filename = "defaultConfig";
    JsonSerializer m_serializer = new JsonSerializer();
    List<string> m_savedFiles = new List<string>();
    int m_selectedIdFiles = 0; 


    void Refresh()
    {
        m_savedFiles.Clear();
        m_savedFiles = JsonSerializer.GetFolderContent();
        m_selectedIdFiles = 0;
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


        if (GUILayout.Button("Refresh List"))
        {
            Refresh();
        }
    }
}
