#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;


layout(location = 0) in vec2 uv_out;

void main() {
	outColor = vec4(uv_out.x,uv_out.y,0.5,1.);
}