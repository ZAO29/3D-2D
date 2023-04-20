using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine;

public class HelloTriangleTuto : MonoBehaviour
{
    [SerializeField]
    Material mat = null;

    void Start()
    {

        Debug.Assert(mat != null);

        // Définition des sommets du triangle
        Vector3[] vertices = new Vector3[3];
        vertices[0] = new Vector3(0, 0, 0);
        vertices[1] = new Vector3(1, 0, 0);
        vertices[2] = new Vector3(0.5f, Mathf.Sqrt(3) / 2f, 0);

        // Définition des indices des sommets pour les triangles du mesh
        int[] triangles = new int[3];
        triangles[0] = 0;
        triangles[1] = 1;
        triangles[2] = 2;

        // Création d'un mesh et assignation de ses sommets et indices
        Mesh mesh = new Mesh();
        mesh.vertices = vertices;
        mesh.triangles = triangles;

        // Assignation du mesh au GameObject actuel
        MeshFilter meshFilter = gameObject.AddComponent<MeshFilter>();
        meshFilter.mesh = mesh;

        // Affichage du mesh en utilisant un composant MeshRenderer
        MeshRenderer meshRenderer = gameObject.AddComponent<MeshRenderer>();
        meshRenderer.material = mat;
    }
}
