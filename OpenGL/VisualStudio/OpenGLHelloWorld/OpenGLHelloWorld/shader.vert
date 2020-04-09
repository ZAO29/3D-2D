#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in float alpha;




void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
	fragColor = vec4(color,alpha);
}