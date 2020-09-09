#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;






uniform mat4 uMVP;
uniform vec3 uBoundingBoxPos;
uniform vec3 uBoundingBoxSize;


void main() {
	vec3 pos = uBoundingBoxPos + uBoundingBoxSize * position;
    gl_Position = uMVP * vec4(pos, 1.0);
}