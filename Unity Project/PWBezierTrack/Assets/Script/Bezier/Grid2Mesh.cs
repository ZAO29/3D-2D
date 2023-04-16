using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;





public class VerticeData<Vec> where Vec : new()
{
    public Vec pos;
    public Vector2 uv;
}



public class Grid2Mesh : MonoBehaviour 
{
   



    static public Mesh Convert(VerticeData<Vector3>[,] grid)
    {

            // Create the mesh
            Mesh mesh = new Mesh();

        var width = grid.GetLength(0);
        var height = grid.GetLength(1);
            // Create the vertices
            Vector3[] vertices = new Vector3[width * height];
            Vector2[] uv = new Vector2[width * height];
        for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {

                    var id = i * height + j;
                    vertices[id] = grid[i,j].pos;
                    uv[id] = grid[i,j].uv;
                }
            }

        // Create the triangles
        List<int> triangles = new List<int>();
            for (int i = 0; i < width - 1; i++)
            {
                for (int j = 0; j < height -1; j++)
                {
                    var idij = i * height + j;
                    var idi1j = (i+1) * height + j;
                    var idij1 = i * height + (j + 1);
                    var idi1j1 = (i + 1) * height + (j + 1);

                    
                    // Culling is one side
                    triangles.Add(idi1j1);
                    triangles.Add(idi1j);
                    triangles.Add(idij);

                //DUPLICATE
                /*triangles.add(idij);
                triangles.add(idi1j);
                triangles.add(idi1j1);*/



                    // culling in the other side
                    triangles.Add(idi1j1);                   
                    triangles.Add(idij);
                    triangles.Add(idij1);

                //DUPLICATE
                /*triangles.Add(idij);
                triangles.Add(idij1);
                triangles.Add(idi1j1);*/



                //so we can always see half of the mesh
                //whatever the side of the camera from the mesh

            }

          }

        // Set the mesh data
        mesh.vertices = vertices;
        mesh.uv = uv;
        mesh.triangles = triangles.ToArray();
       


        return mesh;

    }



    static public GameObject[,] Convert(VerticeData<Vector3>[,][,] grid)
    {
        GameObject[,] objs = new GameObject[grid.GetLength(0),grid.GetLength(1)];
        for (int i =0; i < grid.GetLength(0); i++)
        {
            for (int j = 0; j < grid.GetLength(1); j++)
            {
                var obj = GameObject.CreatePrimitive(PrimitiveType.Plane);
                obj.name = "BS_" + i + "_" + j;
                var m = Grid2Mesh.Convert(grid[i,j]);
                obj.GetComponent<MeshFilter>().mesh = m;

                objs[i,j] = obj;
            }
        }

        return objs;
    }




    static public void DrawGridLR(Vector3[,] ctrlPtGrid, LineRenderer lrPrefab, Transform t)
    {
        for (int i = 0; i < ctrlPtGrid.GetLength(0); i++)
        {
            var lrBSurface = Instantiate(lrPrefab,t);

            lrBSurface.positionCount = ctrlPtGrid.GetLength(1);
            lrBSurface.name = "LR_PWBCtrlPt_w" + i;

            for (int j = 0; j < ctrlPtGrid.GetLength(1); j++)
            {
                lrBSurface.SetPosition(j, ctrlPtGrid[i, j]);
            }
        }


        for (int i = 0; i < ctrlPtGrid.GetLength(1); i++)
        {
            var lrBSurface = Instantiate(lrPrefab, t);

            lrBSurface.positionCount = ctrlPtGrid.GetLength(0);
            lrBSurface.name = "LR_PWBCtrlPt_" + i;

            for (int j = 0; j < ctrlPtGrid.GetLength(0); j++)
            {
                lrBSurface.SetPosition(j, ctrlPtGrid[j, i]);
            }
        }
    }

}
