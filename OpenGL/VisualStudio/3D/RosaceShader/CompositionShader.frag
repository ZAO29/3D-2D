#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;


layout(location = 0) in vec2 uv_out;
layout(binding =0) uniform sampler2D tex0;
layout(binding =1) uniform sampler2D tex1;
layout(binding =2) uniform sampler2D tex2;






void main() {

	outColor = vec4(texture(tex0,uv_out).x,
					texture(tex1,uv_out).x,
					texture(tex2,uv_out).x
					,1.0);
}