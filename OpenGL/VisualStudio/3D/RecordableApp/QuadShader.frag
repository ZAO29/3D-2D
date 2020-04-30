#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;


layout(location = 0) in vec2 uv_out;
layout(binding =0) uniform sampler2D tex0;






void main() {

	vec3 color0 = texture(tex0,uv_out).xyz;
	outColor = vec4(color0,1.0);
}