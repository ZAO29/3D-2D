using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class ApplicationQuit : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Q))
        {
            OnQuitApp();
        }
    }

    public void OnQuitApp()
    {
#if UNITY_EDITOR
        bool decision = EditorUtility.DisplayDialog(
          "Quit Application", // title
          "Are you sure want to quit the app ?", // description
          "Yes", // OK button
          "No" // Cancel button
        );

        if (decision)
        {
            Application.Quit();
            Debug.Log("Exit game");
        }
        else
        {
            Debug.Log("Continue playing");
        }
#else
    Application.Quit();
#endif
    }

}
