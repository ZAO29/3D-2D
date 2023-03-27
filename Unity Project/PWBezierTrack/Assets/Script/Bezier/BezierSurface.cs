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


    public Vec Eval(float u, float v)
    {
        var listCtrlPtU = new List<Vec>();

        for (var i = 0; i < GridCtrlPts.GetLength(0); i++)
        {



            var listCtrlPtV = new List<Vec>();

            for (var j = 0; j < GridCtrlPts.GetLength(1); j++)
            {
                listCtrlPtV.Add(GridCtrlPts[i, j]);
            }

            BezierCurve<Vec> curveV = new BezierCurve<Vec>(listCtrlPtV);
            listCtrlPtU.Add(curveV.Eval(v));
        }

        BezierCurve<Vec> curveU = new BezierCurve<Vec>(listCtrlPtU);
        var ctrlPtU = curveU.Eval(u);
        return ctrlPtU;

    }




    public Vec[,] Sample(int nbPtU, int nbPtV)
    {
        Vec[,] sampled = new Vec[nbPtU, nbPtV];

        for (int iu = 0; iu < nbPtU; iu++)
        {
            float u = ((float)iu) / ((float)nbPtU);
            for (int iv = 0; iv < nbPtV; iv++)
            {
                float v = ((float)iv) / ((float)nbPtV);
                sampled[iu, iv] = this.Eval(u, v);
            }
        }

        return sampled;
    }


}
