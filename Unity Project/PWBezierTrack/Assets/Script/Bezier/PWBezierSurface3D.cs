using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PWBezierSurface3D : MonoBehaviour
{
    BezierSurface<Vector3>[,] gridBSurface;
    public BezierSurface<Vector3>[,] GridBSurface { get => gridBSurface; private set => gridBSurface = value; }

    int NbPieceU { get => GridBSurface.GetLength(0); }
    int NbPieceV { get => GridBSurface.GetLength(1); }


    public LineRenderer lr;

    public PWBezierSurface3D(BezierSurface<Vector3>[,] gridBSurface)
    {
        this.GridBSurface = gridBSurface;
    }

    public VerticeData<Vector3>[,][,] Sample(int nbPtU, int nbPtV)
    {
        VerticeData<Vector3>[,][,] sampled = new VerticeData<Vector3>[NbPieceU, NbPieceV][,];

        for (int iu = 0; iu < NbPieceU; iu++)
        {
            for (int iv = 0; iv < NbPieceV; iv++)
            {
                sampled[iu, iv] = GridBSurface[iu, iv].Sample(nbPtU, nbPtV);
            }
        }

        return sampled;
    }


    public PWBezierSurface3D DerivateU() { return IterateGrid(x => x.DerivateU()); }
    public PWBezierSurface3D DerivateV() { return IterateGrid(x => x.DerivateV()); }

    public PWBezierSurface3D IterateGrid(Func<BezierSurface<Vector3>, BezierSurface<Vector3>> op)
    {
        BezierSurface<Vector3>[,] opGridBSurface = new BezierSurface<Vector3>[NbPieceU, NbPieceV];
        for (int iu = 0; iu < NbPieceU; iu++)
        {
            for (int iv = 0; iv < NbPieceV; iv++)
            {
                opGridBSurface[iu, iv] = op(GridBSurface[iu, iv]);
            }
        }

        return new PWBezierSurface3D(opGridBSurface);
    }

    public void Smoothing()
    {
        Debug.Log("NbPieceU" + NbPieceU);
        Debug.Log("NbPieceV" + NbPieceV);

        for (int i = 0; i < (NbPieceU-1); i++)
        {
            for(int j = 0; j < NbPieceV; j++)
            {
                //GridBSurface[i, j] =  GridBSurface[i, j].Transpose();
                //GridBSurface[i, j] = GridBSurface[i + 1, j].Transpose();

                var bSurfaceIPt = GridBSurface[i, j].GridCtrlPts;
                var bSurfaceI1Pt = GridBSurface[i + 1, j].GridCtrlPts;

                for (int ii = 0; ii < 4; ii++)
                {

                    var der1 = (bSurfaceI1Pt[1, ii] - bSurfaceI1Pt[0, ii]);

                    var der0 = (bSurfaceIPt[3, ii] - bSurfaceIPt[2, ii]);

                    var der = (der1 + der0) / 2.0f;

                    bSurfaceI1Pt[1, ii] = bSurfaceI1Pt[0, ii] + der;

                    bSurfaceIPt[2, ii] = bSurfaceIPt[3, ii] - der;


                    if (Manager.debug)
                    {
                        var debugLR1 = Instantiate(lr);
                        var debugLR2 = Instantiate(lr);

                        debugLR1.name = "debug_LR1";
                        debugLR2.name = "debug_LR2";

                        debugLR1.positionCount = 2;
                        debugLR2.positionCount = 2;

                        debugLR1.SetPosition(0, bSurfaceI1Pt[0, ii]);
                        debugLR1.SetPosition(1, bSurfaceI1Pt[0, ii] + der1);

                        debugLR2.SetPosition(0, bSurfaceIPt[3, ii]);
                        debugLR2.SetPosition(1, bSurfaceIPt[3, ii] - der0);
                    }



                }

                //GridBSurface[i, j] = GridBSurface[i, j].Transpose();
                //GridBSurface[i, j] = GridBSurface[i + 1, j].Transpose();


            }
        }
    }



    public Vector3 Eval(Vector2 uv)
    {
        
        var uvMod = new Vector2(uv.x % (NbPieceU), uv.y % (NbPieceV));
        var idPiece = new Vector2(Mathf.Floor(uvMod.x), Mathf.Floor(uvMod.y));
        var uvBS = uvMod - idPiece;

        
        return GridBSurface[(int)idPiece.x, (int)idPiece.y].Eval(uvBS);
    }



    public PWBezierSurface3D(PWBezierCurve2D track, List<PWBezierCurve2D> sections)
    {
        if(track.nbPiece != (sections.Count -1))
        {
            Debug.LogError("invalid track size vs number sections");
            return;
        }

        int section0Size = sections[0].nbPiece;
        for (int isections = 0; isections < sections.Count; isections++)
        {
            if (sections[isections].nbPiece != section0Size)
            {
                Debug.LogError("section size must be equal " +
                    sections[isections].nbPiece + " vs " +
                    section0Size);

                return;
            }
        }

        this.GridBSurface = new BezierSurface<Vector3>[track.nbPiece,section0Size];


        for (int itrack = 0; itrack < track.nbPiece; itrack++)
        {
            var track2D = track.GetPiece(itrack);
            var derivate2D = track2D.Derivate();
            var derivate0_2D = derivate2D.Eval(0.0f) / 3.0f;
            var derivate1_2D = derivate2D.Eval(1.0f) / 3.0f;

            var p0_2D = track2D.Eval(0.0f);
            var p1_2D = track2D.Eval(1.0f);




            var normal0 = new Vector3(derivate0_2D.y, -derivate0_2D.x, 0).normalized;
            var normal1 = new Vector3(derivate1_2D.y, -derivate1_2D.x, 0).normalized;

            var derivate0 = derivate0_2D.Vec3Z0();
            var derivate1 = derivate1_2D.Vec3Z0();

            var p0 = p0_2D.Vec3Z0();
            var p1 = p1_2D.Vec3Z0();

            var section0 = sections[itrack];
            var section1 = sections[itrack + 1];



            for (int iSection = 0; iSection < section0Size; iSection++)
            {
                var bcurve0 = section0.GetPiece(iSection);
                var bcurve1 = section1.GetPiece(iSection);

                var ctrlPtGrid = new Vector3[4, 4];

                for (int ii = 0; ii < 4; ii++)
                {
                    var pt = bcurve0.CtrlPts[ii];
                    ctrlPtGrid[0, ii] = pt.x * Vector3.forward + pt.y * normal0 + p0;
                    ctrlPtGrid[1, ii] = ctrlPtGrid[0, ii] + derivate0;

                    pt = bcurve1.CtrlPts[ii];
                    ctrlPtGrid[3, ii] = pt.x * Vector3.forward + pt.y * normal1 + p1;
                    ctrlPtGrid[2, ii] = ctrlPtGrid[3, ii] - derivate1;

                }

                this.GridBSurface[itrack, iSection] = new BezierSurface<Vector3>(ctrlPtGrid);
            }
        }

        //Smoothing();
        
    }
}
