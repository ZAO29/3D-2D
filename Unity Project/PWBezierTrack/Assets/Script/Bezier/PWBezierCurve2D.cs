using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PWBezierCurve2D : PWBezierCurve<Vector2>
{
    public PWBezierCurve2D(List<BezierCurve<Vector2>> listBezCurve) : base(listBezCurve)
    {
    }

    public PWBezierCurve2D(List<Vector2> ctrlPt) : base(ctrlPt)
    {

    }


    public PWBezierCurve2D Rotate(float angleDeg)
    {
        var rotatedBezCurves = new List<BezierCurve<Vector2>>();

        for (int iPiece = 0; iPiece < nbPiece; iPiece++)
        {
            var bCurve = this.ListBC[iPiece];
            var rotateCtrlPts = new List<Vector2>();

            for (int iCtrlPt = 0; iCtrlPt < bCurve.nbCtrlPt;iCtrlPt++)
            {
                rotateCtrlPts.Add( bCurve.CtrlPts[iCtrlPt].Rotate(angleDeg) );
            }
            rotatedBezCurves.Add(new BezierCurve<Vector2>(rotateCtrlPts));
        }

        return new PWBezierCurve2D(rotatedBezCurves);
    }





}
