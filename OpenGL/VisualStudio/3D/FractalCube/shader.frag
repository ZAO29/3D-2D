#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;


layout(location = 0) in vec2 uv_out;
layout(binding =0) uniform sampler2D tex;

void main() {
	vec3 color = texture(tex,uv_out).xyz;
	float m = (color.x+color.y+color.z)/3.;
	outColor = vec4(color,m);
}