#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;

layout (location = 0) out float z;




uniform mat4 uMVP;


void main() {
     
    gl_Position = uMVP * vec4(position, 1.0);
	z = position.z; 
}