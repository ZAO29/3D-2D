#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "SkyboxSample.header"


layout(location = 0) in vec3 pos;

layout(location = 0) out vec4 outColor;

uniform vec3 uLightDir = vec3(1.,1.,1.);


//vec4 SkyBoxSample(vec3 position)
//{
//	return vec4(abs(position),1.);
//}

void main() {
	
	outColor = SkyBoxSample(pos);
	outColor = (1.-step(dot(normalize(uLightDir),-pos),0.99))*vec4(1.,1.,1.,1)+outColor;

}