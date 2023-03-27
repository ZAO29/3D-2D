using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using UnityEngine;

public class BezierSurface<Vec> where Vec : new()
{
    Vec[,] gridCtrlPts;

    public Vec[,] GridCtrlPts { get => gridCtrlPts; set => gridCtrlPts = value; }


    public BezierSurface(Vec[,] a_gridCtrlPts)
    {
        GridCtrlPts = a_gridCtrlPts;
    }

    public Vec Eval(Vector2 uv)
    {
        return this.Eval(uv.x, uv.y);
    }

    public Vec Eval(float u, float v)
    {
        var listCtrlPtU = new List<Vec>();

        for (var i = 0; i < GridCtrlPts.GetLength(0); i++)
        {

            BezierCurve<Vec> curveV = GetBCurveLine(i);
            listCtrlPtU.Add(curveV.Eval(v));
        }

        BezierCurve<Vec> curveU = new BezierCurve<Vec>(listCtrlPtU);
        var ctrlPtU = curveU.Eval(u);
        return ctrlPtU;

    }

    private BezierCurve<Vec> GetBCurveLine(int idLine)
    {
        var listCtrlPtV = new List<Vec>();

        for (var j = 0; j < GridCtrlPts.GetLength(1); j++)
        {
            listCtrlPtV.Add(GridCtrlPts[idLine, j]);
        }

        BezierCurve<Vec> curveV = new BezierCurve<Vec>(listCtrlPtV);
        return curveV;
    }



    public Vec[,] Sample(int nbPtU, int nbPtV)
    {
        Vec[,] sampled = new Vec[nbPtU, nbPtV];

        for (int iu = 0; iu < nbPtU; iu++)
        {
            float u = ((float)iu) / ((float)nbPtU - 1.0f);
            for (int iv = 0; iv < nbPtV; iv++)
            {
                float v = ((float)iv) / ((float)nbPtV - 1.0f);
                sampled[iu, iv] = this.Eval(u, v);
            }
        }

        return sampled;
    }




    private BezierSurface<Vec> Transpose()
    {
        Vec[,] transposedCtrlPt = new Vec[this.GridCtrlPts.GetLength(1), 
                                          this.GridCtrlPts.GetLength(0)];

        for (int i = 0; i < transposedCtrlPt.GetLength(0); i++)
        {
            for (int j = 0; j < transposedCtrlPt.GetLength(1); j++)
            {
                transposedCtrlPt[i, j] = this.GridCtrlPts[j, i];
            }
        }

        return new BezierSurface<Vec>(transposedCtrlPt);

    }


    public BezierSurface<Vec> DerivateV()
    {
        Vec[,] derVCtrlPt = new Vec[this.GridCtrlPts.GetLength(0),
                                          this.GridCtrlPts.GetLength(1)-1];

        for (var i = 0; i < GridCtrlPts.GetLength(0); i++)
        {

            BezierCurve<Vec> curveV = GetBCurveLine(i);
            var derCurveV = curveV.Derivate();

            for (var j = 0; j < derCurveV.nbCtrlPt;j++)
            {
                derVCtrlPt[i, j] = derCurveV.CtrlPts[j];
            }
        }

        BezierSurface<Vec> derV = new BezierSurface<Vec>(derVCtrlPt);

        return derV;
    }


    public BezierSurface<Vec> DerivateU()
    {
        var transpose = this.Transpose();

        var derTranspose = transpose.DerivateV();

        return derTranspose.Transpose();
    }



}
