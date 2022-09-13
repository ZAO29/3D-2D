using UnityEngine;
using System;
public class TrackBallCam : MonoBehaviour
{
    Vector3 m_init_pos;
    Vector3 m_init_euler;

    Vector3 m_mouseLastPos;

    [SerializeField]
    bool m_center = true;

    private void Start()
    {
        InitCamPos();
    }


    void InitCamPos()
    {
        var obj = GameObject.Find("RootNode");
        obj.transform.position = new Vector3(0, 0, 0);
        Bounds b = new Bounds();
        var first = true;

        var camDir = Camera.main.transform.forward;

        if (m_center)
        {
            foreach (var r in obj.GetComponentsInChildren<Renderer>())
            {
                Debug.Log("bounds c : " + r.bounds.center.ToString() + " " + r.bounds.extents);
                if (first)
                {
                    first = false;
                    b = new Bounds(r.bounds.center, r.bounds.size);

                }
                else
                {
                    b.Encapsulate(r.bounds);
                }

            }

            obj.transform.position = -b.center;
        }
        var maxsize = Mathf.Max(b.extents.x, b.extents.y, b.extents.z);
        //Camera.main.transform.position = 1.5f * new Vector3(-b.extents.x, b.extents.y, -b.extents.z);
        Camera.main.transform.position = -2.5f * maxsize *camDir;

        Camera.main.transform.LookAt(new Vector3(0, 0, 0));

        m_init_pos = Camera.main.transform.position;
        m_init_euler = Camera.main.transform.eulerAngles;

    }

#if UNITY_STANDALONE
    private void Update()
    {
        int screenHeight = Screen.height;
        int screenWidth = Screen.width;

        int id_MouseButton = 0;
        if(Input.GetMouseButtonDown(id_MouseButton))
        {
            m_mouseLastPos = Input.mousePosition;
        }else if (Input.GetMouseButton(id_MouseButton))
        {
            var mousePos = Input.mousePosition;
            var delta = mousePos - m_mouseLastPos;
            m_mouseLastPos = mousePos;
            float angleW = (180 * delta.x) / screenWidth;
            float angleP = (180 * delta.y) / screenHeight;
            //add the restrications of roation angle in y direction


            //Warning: the eulerAngles get from here may not the same with the value in inspector!
            //https://docs.unity3d.com/Manual/QuaternionAndEulerRotationsInUnity.html
            //var angle = new Vector3(Camera.main.transform.eulerAngles.x + angleP, Camera.main.transform.eulerAngles.y + angleW, 0);

            Camera.main.transform.RotateAround(new Vector3(0, 0, 0), Camera.main.transform.right, angleP);
            Camera.main.transform.RotateAround(new Vector3(0, 0, 0), Camera.main.transform.up, angleW);
        }

        if(Math.Abs(Input.mouseScrollDelta.y) > 0)
        {
            Debug.Log(Input.mouseScrollDelta.y);
            Camera.main.transform.transform.position = Camera.main.transform.transform.position / (1 + Input.mouseScrollDelta.y/10);
        }
    }
#endif


#if UNITY_IOS || UNITY_ANDROID
    private void Update()
    {
        int screenHeight = Screen.height;
        int screenWidth = Screen.width;
        
        //Scroll
        if (Input.touchCount == 1)
        {
            if (Input.GetTouch(0).phase == TouchPhase.Moved)
            {
                Vector2 delta = Input.GetTouch(0).deltaPosition;
                float angleW = (180 * delta.x) / screenWidth;
                float angleP = (180 * delta.y) / screenHeight;
                //add the restrications of roation angle in y direction


                //Warning: the eulerAngles get from here may not the same with the value in inspector!
                //https://docs.unity3d.com/Manual/QuaternionAndEulerRotationsInUnity.html
                //var angle = new Vector3(Camera.main.transform.eulerAngles.x + angleP, Camera.main.transform.eulerAngles.y + angleW, 0);

                Camera.main.transform.RotateAround(new Vector3(0, 0, 0), Camera.main.transform.right, angleP);
                Camera.main.transform.RotateAround(new Vector3(0, 0, 0), Camera.main.transform.up, angleW);

               // float n = Camera.main.transform.position.magnitude;
               //Camera.main.transform.eulerAngles = angle;
               //Camera.main.transform.position = - n * Camera.main.transform.forward.normalized; 

            }
        }

        //Pinch
        if (Input.touchCount == 2)
        {
            if (Input.GetTouch(0).phase == TouchPhase.Moved)
            {
                Vector2 pos1 = Input.GetTouch(0).position;
                Vector2 pos2 = Input.GetTouch(1).position;
                Vector2 pos1b = Input.GetTouch(0).position - Input.GetTouch(0).deltaPosition;
                Vector2 pos2b = Input.GetTouch(1).position - Input.GetTouch(1).deltaPosition;

                float firstDistance = Vector2.Distance(pos1, pos2);
                float secondDistance = Vector2.Distance(pos1b, pos2b);
                float diff = firstDistance - secondDistance;

                Camera.main.transform.transform.position = Camera.main.transform.transform.position / (1 + (diff / screenWidth));
            }
        }

        //reset
        if (Input.touchCount == 3)
        {
            ResetView();
        }
    }
#endif

    public void ResetView()
    {
        Camera.main.transform.position = m_init_pos;
        Camera.main.transform.eulerAngles = m_init_euler;
    }
}