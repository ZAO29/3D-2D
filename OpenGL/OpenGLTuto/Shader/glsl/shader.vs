#version 460

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;

out vec4 Color;

uniform mat4 gMVP; 
uniform mat4 gWorld;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;
out vec3 Tangent0;

void main()
{
 
	
    gl_Position = gMVP * vec4(Position.x,Position.y, Position.z, 1.0);

    Color = vec4(clamp(Position, 0.0 , 1.0),1.0);
    TexCoord0 = TexCoord;
    
    Normal0 = normalize((gWorld*vec4(Normal,1.0)).xyz);
    WorldPos0 = (gWorld*vec4(Position,1.0)).xyz;
    Tangent0 = (gWorld*vec4(Tangent,1.0)).xyz;
}
