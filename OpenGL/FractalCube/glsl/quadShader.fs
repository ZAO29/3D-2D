#version 460

layout (location = 0) out vec3 FragColor;


in vec2 uv;


layout(binding =0) uniform sampler2D tex;






void main()
{

        FragColor = texture(tex,uv).xyz;

    

    
	
}
