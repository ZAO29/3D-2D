using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Runtime.Remoting.Messaging;
using UnityEngine;

public class PWBezierCurve<Vec> where Vec : new()
{

    List<BezierCurve<Vec>> m_listBC = new List<BezierCurve<Vec>>();
    public List<BezierCurve<Vec>> ListBC { get => m_listBC; set => m_listBC = value; }

    public int nbPiece { get => ListBC.Count; }

    public PWBezierCurve(List<BezierCurve<Vec>> listBezCurve)
    {
            ListBC = listBezCurve;
    }


    public PWBezierCurve(List<Vec> listCtrlPt)
    {

        float nbPiece = (listCtrlPt.Count - 1) / 3.0f;

        if(nbPiece != Mathf.Floor(nbPiece))
        {
            Debug.LogError("PWBCurve invalid intial setup of ctrlPt");
            return;
        }



        for (int idPiece = 0; idPiece < nbPiece; idPiece++)
        {
            var ctrlPt = listCtrlPt.GetRange(idPiece * 3,4);
            ListBC.Add(new BezierCurve<Vec>(ctrlPt));
        }
    }

    public BezierCurve<Vec> GetPiece(int idPiece)
    {
        if(idPiece < 0 || idPiece >= nbPiece)
        {
            Debug.LogError("PWBezCurve invalid idPiece " + idPiece);
            return null;
        }

        return m_listBC[idPiece];
    }

    public Vec Eval(float t)
    {
        int idPiece = Mathf.FloorToInt(t);

        if(t == nbPiece)
        {
            idPiece = (idPiece - 1);
        }

        float remainder = t - idPiece;
        
        var bcurve = GetPiece(idPiece);

        return bcurve.Eval(remainder);
    }



    public List<Vec> Sample(int nbPt)
    {
        List<Vec> sample = new List<Vec>();

        for (int i = 0; i < nbPt; i++)
        {
            float t = this.nbPiece * ((float)i) / ((float)nbPt - 1);
            sample.Add(this.Eval(t));
        }

        return sample;
    }




}
