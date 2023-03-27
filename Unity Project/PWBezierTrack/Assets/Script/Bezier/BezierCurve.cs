using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using UnityEngine;

public class BezierCurve<Vec> where Vec : new()
{
    List<Vec> ctrlPts;

    public int degree { get => CtrlPts.Count - 1; }

    public int nbCtrlPt { get => CtrlPts.Count; }

    public List<Vec> CtrlPts { get => ctrlPts; set => ctrlPts = value; }

    public BezierCurve(List<Vec> initCtrlPt)
    {
        this.CtrlPts = initCtrlPt;
    }


   public static float BinomialCoeff(int k, int n)
    {
        return ((float) factorial(n)) / ((float) factorial(k) * factorial(n - k));
    }


    public static int factorial(int n)
    {
        if (n == 0)
            return 1;
        else
            return n * factorial(n - 1);
    }


    static float Bernstein(float t, int k, int n)
    {
        return BinomialCoeff(k, n)*Mathf.Pow(t, k)* Mathf.Pow(1.0f - t, (n-k));
    }

    public Vec Eval(float t)
    {
        var val = new Vec();
        int k = 0;

        foreach(var ctrlPt in this.CtrlPts)
        {
            var ctrlPtValue = ctrlPt as dynamic;
            val += Bernstein(t, k, this.degree) * ctrlPtValue;
            k++;
        }

        return val;
    }


    public List<Vec> Sample(int nbPt)
    {
        List<Vec> sample = new List<Vec>();


        for (int i = 0; i < nbPt; i++)
        {
            float t = ((float) i) / ((float) nbPt-1);
            sample.Add(Eval(t));
        }

        return sample;
    }


    public BezierCurve<Vec> Derivate()
    {
        List<Vec> derCtrlPts = new List<Vec>();

        for (var i = 0; i < degree; i++)
        {
            var ctrlPtI = ctrlPts[i] as dynamic;
            var ctrlPtI1 = ctrlPts[i+1] as dynamic;
            var derCtrlPt = ((float) degree) * (ctrlPtI1 - ctrlPtI);
            derCtrlPts.Add(derCtrlPt);
        }

        return new BezierCurve<Vec>(derCtrlPts);
    }
        

}
