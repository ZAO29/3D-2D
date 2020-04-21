#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) out vec4 FragColor;

in vec2 uv;

layout(binding =0) uniform sampler2D tex;

void main()
{
        FragColor = vec4(texture(tex,uv).xyz,1.0);
		//FragColor = vec4(1.,0.,0.,1.0);
}