#version 450
#extension GL_ARB_separate_shader_objects : enable


layout (location = 0) in vec2 TexCoord;

out vec2 uv;




void main()
{	
    gl_Position = vec4(TexCoord.x,TexCoord.y, 1.0, 1.0);
    uv = TexCoord;
}
