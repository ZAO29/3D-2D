using UnityEngine;

public class FractalCubeRenderer : MonoBehaviour
{
    public int numberOfCubes = 5;

    private RenderTexture renderTexture;

    [SerializeField] Texture initialTexture;




    public Material material;

    public Material material2;


    Mesh mesh;

    private void Start()
    {
        renderTexture = new RenderTexture(256, 256, 24);
        renderTexture.Create();
        mesh = GetComponent<MeshFilter>().sharedMesh;
        material = GetComponent<MeshRenderer>().material;
        material.mainTexture = initialTexture;
        material2 = new Material(material);
    }

    private void LateUpdate()
    {
        
            // Bind the first render texture
            Graphics.SetRenderTarget(renderTexture);
            

            GL.Clear(true, true, Color.black);
            GL.PushMatrix();

            material.SetPass(0);
            Graphics.DrawMeshNow(mesh, this.transform.localToWorldMatrix);

            GL.PopMatrix();



            // Bind the current texture to use as input for the shader
            material2.mainTexture = renderTexture;
            Graphics.SetRenderTarget((RenderTexture)null);
            GL.Clear(true, true, Color.red);
            GL.PushMatrix();

            // Draw the cube with the material
            //material.SetPass(1);
            Graphics.DrawMeshNow(mesh, this.transform.localToWorldMatrix);

            GL.PopMatrix();

        GL.Clear(true, true, Color.black);

       
    }
}

