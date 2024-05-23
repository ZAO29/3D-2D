
 

using UnityEngine;
using System;
public class YPRCam : MonoBehaviour
{
    Vector3 m_init_pos;
    Vector3 m_init_euler;

    Vector3 m_mouseLastPos;

    [SerializeField]
    bool m_center = true;

    [SerializeField]
    float rollSpeed;

    [SerializeField]
    float zoomSpeed = 0.99f;

    private void Start()
    {
        InitCamPos();
    }


    void InitCamPos()
    {
    }




#if UNITY_STANDALONE
    private void Update()
    {

        Vector3 deltaYPR = Vector3.zero;
        int screenHeight = Screen.height;
        int screenWidth = Screen.width;

        int id_MouseButton = 0;
        if (Input.GetMouseButtonDown(id_MouseButton))
        {
            m_mouseLastPos = Input.mousePosition;
        }
        else if (Input.GetMouseButton(id_MouseButton))
        {
            var mousePos = Input.mousePosition;
            var delta = mousePos - m_mouseLastPos;
            m_mouseLastPos = mousePos;
            deltaYPR.y = (180 * delta.x) / screenWidth;
            deltaYPR.x = (180 * delta.y) / screenHeight;
            //add the restrications of roation angle in y direction


            //Warning: the eulerAngles get from here may not the same with the value in inspector!
            //https://docs.unity3d.com/Manual/QuaternionAndEulerRotationsInUnity.html
            //var angle = new Vector3(Camera.main.transform.eulerAngles.x + angleP, Camera.main.transform.eulerAngles.y + angleW, 0);

            
        }


        if(Input.GetKey(KeyCode.R))
        {
            deltaYPR.z = rollSpeed * Time.deltaTime;
        }


        if (Input.GetKey(KeyCode.T))
        {
            deltaYPR.z -= rollSpeed;
        }


        if (Input.GetKey(KeyCode.Z))
        {

            this.transform.position = zoomSpeed * new Vector3(this.transform.position.x,
                                                           this.transform.position.y,
                                                           this.transform.position.z);
        }

        if (Input.GetKey(KeyCode.S))
        {

            this.transform.position = 1.0f/zoomSpeed * new Vector3(this.transform.position.x,
                                                           this.transform.position.y,
                                                           this.transform.position.z);
        }


        this.transform.eulerAngles += deltaYPR;
        this.transform.position = - this.transform.position.magnitude * this.transform.forward;

        if (Math.Abs(Input.mouseScrollDelta.y) > 0)
        {
            Camera.main.transform.transform.position = Camera.main.transform.transform.position / (1 + Input.mouseScrollDelta.y / 10);
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
