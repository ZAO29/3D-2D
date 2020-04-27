#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec3 outColor;


layout(location = 0) in vec2 uv_out;
layout(binding =0) uniform sampler2D tex;

void main() {
	outColor = texture(tex,uv_out).xyz;
}