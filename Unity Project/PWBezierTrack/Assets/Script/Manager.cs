using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Manager : MonoBehaviour
{
    [SerializeField]
    LineRenderer lrPrefab;






    // Start is called before the first frame update
    void Start()
    {
        //PWBezCurveTest();
        BezierSurfaceTest();
    }



    void BCurveTest()
    {
        Debug.Assert(BezierCurve<Vector3>.BinomialCoeff(1, 5) == 5);
        Debug.Assert(BezierCurve<Vector3>.BinomialCoeff(4, 5) == 5);


        List<Vector3> ctrlPts = new List<Vector3>();
        ctrlPts.Add(new Vector3(0, 0, 0));
        ctrlPts.Add(new Vector3(0, 0, 1));
        ctrlPts.Add(new Vector3(0, 1, 1));
        ctrlPts.Add(new Vector3(1, 1, 1));





        BezierCurve<Vector3> bezcurve3D = new BezierCurve<Vector3>(ctrlPts);

        Debug.Assert(bezcurve3D.Eval(0) == bezcurve3D.CtrlPts[0]);
        Debug.Assert(bezcurve3D.Eval(1) == bezcurve3D.CtrlPts[bezcurve3D.degree]);

        var lrBCtrlPt = Instantiate(lrPrefab, this.transform);

        lrBCtrlPt.positionCount = bezcurve3D.CtrlPts.Count;
        lrBCtrlPt.SetPositions(bezcurve3D.CtrlPts.ToArray());
        lrBCtrlPt.material.color = Color.red;


        var lrBCurve = Instantiate(lrPrefab, this.transform);

        var nbPt = 100;
        var samplesPts = bezcurve3D.Sample(nbPt);
        lrBCurve.positionCount = nbPt;
        lrBCurve.SetPositions(samplesPts.ToArray());
    }



    void PWBezCurveTest()
    {
        List<Vector3> PWctrlPts = new List<Vector3>();
        PWctrlPts.Add(new Vector3(0, 0, 0));// 0
        PWctrlPts.Add(new Vector3(0, 0, 1));
        PWctrlPts.Add(new Vector3(0, 1, 1));
        PWctrlPts.Add(new Vector3(1, 1, 1)); // 3
        PWctrlPts.Add(new Vector3(2, 1, 1));
        PWctrlPts.Add(new Vector3(2, 2, 1));
        PWctrlPts.Add(new Vector3(2, 2, 2)); // 6
        PWctrlPts.Add(new Vector3(3, 2, 2));
        PWctrlPts.Add(new Vector3(3, 2, 2));
        PWctrlPts.Add(new Vector3(4, 2, 2)); // 9

        PWBezierCurve<Vector3> pwbcurve = new PWBezierCurve<Vector3>(PWctrlPts);

        var nbPt = 100;


        var lrPWBcurve = Instantiate(lrPrefab, this.transform);


        var samplesPts = pwbcurve.Sample(nbPt);
        lrPWBcurve.positionCount = nbPt;
        lrPWBcurve.SetPositions(samplesPts.ToArray());
        lrPWBcurve.name = "LR_PWBCurve";
        lrPWBcurve.material.color = Color.blue;



        var lrPWCtrlPt = Instantiate(lrPrefab, this.transform);

        lrPWCtrlPt.positionCount = PWctrlPts.Count;
        lrPWCtrlPt.SetPositions(PWctrlPts.ToArray());
        lrPWCtrlPt.name = "LR_PWBCtrlPt";
    }

    void BezierSurfaceTest()
    {
        var obj = GameObject.CreatePrimitive(PrimitiveType.Plane);

        Vector3[,] ctrlPtGrid = new Vector3[3, 4]
        {
            { new Vector3(0, 0, 0), new Vector3(0, 1, 0), new Vector3(0, 2, 0), new Vector3(0, 3, 0) },
            { new Vector3(1, 0, 1), new Vector3(1, 1, 2), new Vector3(1, 2, 2), new Vector3(1, 3, 1) },
            { new Vector3(2, 0, 0), new Vector3(2, 1, -1), new Vector3(2, 2, -1), new Vector3(2, 3, 0) }
        };

        var bSurface = new BezierSurface<Vector3>(ctrlPtGrid);

        var sampled = bSurface.Sample(30, 40);

        var m = Grid2Mesh.Convert(sampled);

        obj.GetComponent<MeshFilter>().mesh = m;
        var mc = obj.GetComponent<MeshCollider>();
        Destroy(mc);
        obj.AddComponent<MeshCollider>();
        obj.name = "BSurface";


        for (int i = 0; i < ctrlPtGrid.GetLength(0); i++)
        {
            var lrBSurface = Instantiate(lrPrefab, this.transform);

            lrBSurface.positionCount = ctrlPtGrid.GetLength(1);
            lrBSurface.name = "LR_PWBCtrlPt_w" + i;

            for (int j = 0; j < ctrlPtGrid.GetLength(1); j++)
            {
                lrBSurface.SetPosition(j,  ctrlPtGrid[i,j]);
            }
        }


        for (int i = 0; i < ctrlPtGrid.GetLength(1); i++)
        {
            var lrBSurface = Instantiate(lrPrefab, this.transform);

            lrBSurface.positionCount = ctrlPtGrid.GetLength(0);
            lrBSurface.name = "LR_PWBCtrlPt_" + i;

            for (int j = 0; j < ctrlPtGrid.GetLength(0); j++)
            {
                lrBSurface.SetPosition(j, ctrlPtGrid[j, i]);
            }
        }


        // Vector2 uv = new Vector2(0.25f, 0.75f);
        var uvs = new List<Vector2>()
        { 
                    new Vector2(1.0f, 0.0f),
                    new Vector2(0.0f, 1.0f),
                    new Vector2(0.0f, 0.0f),
                    new Vector2(1.0f, 1.0f),
                    new Vector2(0.5f, 0.5f) 
        };



        for (int iuv = 0; iuv < uvs.Count; iuv++)
        {
            var uv = uvs[iuv];
            var derUSurface = bSurface.DerivateU();
            var derVSurface = bSurface.DerivateV();

            var pt = bSurface.Eval(uv);

            var derU = derUSurface.Eval(uv).normalized * 0.5f;
            var derV = derVSurface.Eval(uv).normalized * 0.5f;
            var normal = Vector3.Cross(derU, derV).normalized * 0.5f;

            var colors = new List<Color>() { Color.red, Color.green, Color.blue };

            var ptDirs = new List<PtDir>() { new PtDir(pt, derU, Color.cyan), new PtDir(pt, derV, Color.magenta), new PtDir(pt, normal, Color.black) };


            for (int iFrame = 0; iFrame < ptDirs.Count; iFrame++)
            {
                var ptDir = ptDirs[iFrame];
                var lrFrame = Instantiate(lrPrefab, this.transform);
                lrFrame.name = "frame_" + iuv + "_" + iFrame;
                lrFrame.SetPosition(0, ptDir.pt);
                lrFrame.SetPosition(1, ptDir.pt + ptDir.dir);
                lrFrame.material.color = ptDir.color;
            }
        }

    }


    struct PtDir
    {
        public Vector3 pt;
        public Vector3 dir;
        public Color color;

        public PtDir(Vector3 pt, 
                     Vector3 dir,
                     Color color)
        {
            this.pt = pt;
            this.dir = dir;
            this.color = color;

        }
    }



}
