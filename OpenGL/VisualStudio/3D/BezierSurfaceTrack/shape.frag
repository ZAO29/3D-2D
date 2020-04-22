#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 fragColor_out;

layout(location = 0) out vec4 outColor;

uniform float uTime;

void main() {
    float a = cos(10.*fragColor_out.x*uTime);
	outColor = vec4(a,a,a,1.0);
	//outColor = vec4(1.,0.,0.,1.0);
}