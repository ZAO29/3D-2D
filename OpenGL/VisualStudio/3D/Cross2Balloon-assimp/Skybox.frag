#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 pos;

layout(location = 0) out vec4 outColor;

uniform vec3 uLightDir = vec3(1.,1.,1.);

void main() {
	
	outColor = vec4(abs(pos),1.);
	outColor = (1.-step(dot(normalize(uLightDir),-pos),0.99))*vec4(1.,1.,1.,1)+outColor;

}