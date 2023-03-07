using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IHMVisibility : MonoBehaviour
{
    [SerializeField]
    Canvas UI_canvas = null;
    // Start is called before the first frame update
    [SerializeField] 
    bool visible = false;

    public bool Visible { get => visible;

        set
        {
            visible = value;
            SetVisibility(visible);
        }
    }

    void Start()
    {
        Debug.Assert(UI_canvas != null);

        //force l'update de l'etat
        Visible = Visible;
    }

    // Update is called once per frame
    void Update()
    {
        if(UI_canvas == null) return;

        if(Input.GetKeyDown(KeyCode.V))
        {
            Visible = !Visible;
            
        }
    }


    void SetVisibility(bool visible)
    {
        for (int i = 0; i < UI_canvas.gameObject.transform.childCount; i++)
        {
            var child = UI_canvas.gameObject.transform.GetChild(i);
            child.gameObject.SetActive(visible);
        }
    }
}
