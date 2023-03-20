using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Manager : MonoBehaviour
{
    [SerializeField]
    LineRenderer lr;

    [SerializeField]
    LineRenderer lrCtrlPt;



    // Start is called before the first frame update
    void Start()
    {
        Debug.Assert(BezierCurve<Vector3>.BinomialCoeff(1, 5) == 5);
        Debug.Assert(BezierCurve<Vector3>.BinomialCoeff(4, 5) == 5);


        List<Vector3> ctrlPts = new List<Vector3>();
        ctrlPts.Add(new Vector3(0, 0, 0));
        ctrlPts.Add(new Vector3(0, 0, 1));
        ctrlPts.Add(new Vector3(0, 1, 1));
        ctrlPts.Add(new Vector3(1, 1, 1));





        BezierCurve<Vector3> bezcurve3D = new BezierCurve<Vector3>(ctrlPts);

        lrCtrlPt.positionCount = bezcurve3D.CtrlPts.Count ;
        lrCtrlPt.SetPositions(bezcurve3D.CtrlPts.ToArray());


        Debug.Assert(bezcurve3D.Eval(0) == bezcurve3D.CtrlPts[0]);
        Debug.Assert(bezcurve3D.Eval(1) == bezcurve3D.CtrlPts[bezcurve3D.degree]);

        var nbPt = 100;
        var samplesPts = bezcurve3D.Sample(nbPt);
        lr.positionCount = nbPt;
        lr.SetPositions(samplesPts.ToArray());
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
