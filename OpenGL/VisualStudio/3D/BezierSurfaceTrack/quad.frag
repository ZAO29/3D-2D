#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) out vec4 FragColor;

in vec2 uv;
uniform bool uUseTex;

layout(binding =0) uniform sampler2D tex;


void main()
{
        if(uUseTex)
		{	
			FragColor = texture(tex,uv).xyzw;
		}else
		{
			FragColor = vec4(1.0,1.0,1.0,1.0);
		}
		//FragColor = vec4(1.,0.,0.,1.0);
}