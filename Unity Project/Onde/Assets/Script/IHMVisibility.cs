using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IHMVisibility : MonoBehaviour
{
    [SerializeField]
    Canvas UI_canvas = null;
    // Start is called before the first frame update
    void Start()
    {
        Debug.Assert(UI_canvas != null);
    }

    // Update is called once per frame
    void Update()
    {
        if(UI_canvas == null) return;

        if(Input.GetKeyDown(KeyCode.V))
        {
            UI_canvas.gameObject.SetActive(!UI_canvas.gameObject.activeInHierarchy);
        }
    }
}
