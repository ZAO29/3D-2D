#version 460

layout (location = 0) out vec3 FragColor;


//in vec2 uv;
in VS_OUT {
    vec2 uv;
} vs_out;

layout(binding =0) uniform sampler2D tex;






void main()
{
          //FragColor = vec3(1.,0.,0.);
        //FragColor = vec3(uv.x,uv.y,0.);
        FragColor = vec3(vs_out.uv.x,vs_out.uv.y,0.);

    

    
	
}
