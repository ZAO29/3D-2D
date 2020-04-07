#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec3 fragColor;

layout (location = 0) in vec3 position;




void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
	fragColor = vec3(position.y,position.x,position.z);
}