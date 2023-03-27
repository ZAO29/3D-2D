using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class Grid2Mesh 
{
   
    static public Mesh Convert(Vector3[,] grid)
    {

            // Create the mesh
            Mesh mesh = new Mesh();

        var width = grid.GetLength(0);
        var height = grid.GetLength(1);
            // Create the vertices
            Vector3[] vertices = new Vector3[width * height];
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {

                    var id = i * height + j;
                    Debug.Log(" id " + id + " i " + i + " j " + j);
                    vertices[id] = grid[i,j];
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
                    triangles.Add(idij1);
                    triangles.Add(idij);

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
            mesh.triangles = triangles.ToArray();


        return mesh;

    }
}
