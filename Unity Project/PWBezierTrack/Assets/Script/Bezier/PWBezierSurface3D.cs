using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PWBezierSurface3D 
{
    BezierSurface<Vector3>[,] gridBSurface;
    public BezierSurface<Vector3>[,] GridBSurface { get => gridBSurface; private set => gridBSurface = value; }

    int NbPieceU { get => GridBSurface.GetLength(0); }
    int NbPieceV { get => GridBSurface.GetLength(1); }
    

    public PWBezierSurface3D(BezierSurface<Vector3>[,] gridBSurface)
    {
        this.GridBSurface = gridBSurface;
    }

    public Vector3[,][,] Sample(int nbPtU, int nbPtV)
    {
        Vector3[,][,] sampled = new Vector3[NbPieceU, NbPieceV][,];

        for (int iu = 0; iu < NbPieceU; iu++)
        {
            for (int iv = 0; iv < NbPieceV; iv++)
            {
                sampled[iu, iv] = GridBSurface[iu, iv].Sample(nbPtU, nbPtV);
            }
        }

        return sampled;
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
        
    }
}
