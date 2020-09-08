#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 0) out float z;
layout (location = 1) out vec3 normalWorld;
layout (location = 2) out vec3 posWorld;




uniform mat4 uMVP;
uniform mat4 uModel;


void main() {
     
    gl_Position = uMVP * vec4(position, 1.0);
	normalWorld = normalize((uModel * vec4(normal, 1.0)).xyz);
	posWorld = (uModel * vec4(position, 1.0)).xyz;
	z = position.z; 
}