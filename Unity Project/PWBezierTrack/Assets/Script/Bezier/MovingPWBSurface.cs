using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class MovingPWBSurface : MonoBehaviour
{
    [SerializeField] float speed = 0.5f;

    PWBezierSurface3D surf;
    PWBezierSurface3D[] surfaceDerS = new PWBezierSurface3D[2];

    Vector2 uv;

    Vector2 forwardUV = new Vector2(1, 0);
   
    public float Speed { get => speed; set => speed = value; }
    public PWBezierSurface3D Surf { get => surf; set => surf = value; }
    public PWBezierSurface3D[] SurfaceDerS { get => surfaceDerS; set => surfaceDerS = value; }
    
    public Vector2 UV { get => uv; set => uv = value; }
    public Vector2 ForwardUV { get => forwardUV; set => forwardUV = value; }

    Vector2 RightUV { get => Vector2.one - ForwardUV; }


    [SerializeField] float offset = 0.25f;

    public void Init(PWBezierSurface3D surf, Vector2 uv = default(Vector2))
    {
        Surf = surf;
        SurfaceDerS[0] = surf.DerivateU();
        SurfaceDerS[1] = surf.DerivateV();

        UV = uv;
    }

    // Update is called once per frame
    void Update()
    {
        var deltaTime = Time.deltaTime;

        UV += deltaTime * ForwardUV * speed;

        var pos = Surf.Eval(UV);

        var idForward = ForwardUV.IdOne();
        var idRight = 1 - idForward;

        var rightV = SurfaceDerS[idRight].Eval(UV);
        var forwardV = SurfaceDerS[idForward].Eval(UV);

        var upV = Vector3.Cross(rightV, forwardV).normalized;

        rightV = Vector3.Cross(forwardV, upV).normalized;

        this.transform.position = pos + offset * upV;
    

        this.transform.LookAt(pos + forwardV, upV);

    }
}
