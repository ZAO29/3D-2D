#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 0) out vec3 pos_out;
layout (location = 1) out vec3 normal_out;








void main() {
     
	pos_out = position;
	normal_out = normal;
}