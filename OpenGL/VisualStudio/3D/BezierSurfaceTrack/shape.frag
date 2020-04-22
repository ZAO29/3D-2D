#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 fragColor_out;
layout(location = 1) in float z;

layout(location = 0) out vec4 outColor;

uniform float uTime;
uniform float uVisi;

void main() {
    float a = cos(10.*fragColor_out.x*uTime);
	outColor = vec4(a,a,a,1.0)*exp(-z*uVisi)+(1-exp(-z*uVisi))*vec4(1.,1.,1.,1.);
	//outColor = vec4(1.,0.,0.,1.0);
}