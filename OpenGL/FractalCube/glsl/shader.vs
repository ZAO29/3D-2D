#version 460

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;


out vec2 uv;

uniform mat4 gMVP; 


void main()
{	
    gl_Position = gMVP * vec4(Position.x,Position.y, Position.z, 1.0);
    uv = TexCoord;
}
