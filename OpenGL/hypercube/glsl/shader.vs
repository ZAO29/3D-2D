#version 460

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TexCoord;


out vec2 uv;

uniform mat4 gMVP; 


void main()
{	

    vec3 cPosition = vec3(3.0,0.0,0.0) + vec3(Position.x,Position.y,Position.z);
     cPosition.xyz *= 1.0/Position.w;
    gl_Position = gMVP * vec4(cPosition.x,cPosition.y, cPosition.z, 1.0);
    uv = TexCoord;
}
