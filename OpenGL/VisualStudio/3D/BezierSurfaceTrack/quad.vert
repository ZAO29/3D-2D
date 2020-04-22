#version 450
#extension GL_ARB_separate_shader_objects : enable


layout (location = 0) in vec2 TexCoord;

out vec2 uv;

uniform vec4 uOffsetScale;


void main()
{	
	vec2 coord = TexCoord;
	coord -= uOffsetScale.xy;
	coord *= uOffsetScale.zw;
    gl_Position = vec4(coord.x,coord.y, 1.0, 1.0);
    uv = TexCoord;
}
