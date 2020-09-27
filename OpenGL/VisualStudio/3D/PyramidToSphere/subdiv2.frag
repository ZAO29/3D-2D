#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;




layout (location = 0) in flat int colorID;
layout(location = 1) in vec3  normal;




vec3 lightDir = normalize(vec3(1.,-1.,1.));

vec4 colorsPerLevel[10] = 
{
vec4(1.,0.,0.,1.),//1
vec4(0.,1.,0.,1.),//2
vec4(0.,0.,1.,1.),//3
vec4(0.,1.,1.,1.),//4
vec4(1.,0.,1.,1.),//5
vec4(1.,1.,0.,1.),//6
vec4(1.,1.,1.,1.),//7
vec4(1.,1.,0.5,1.),//8
vec4(1.,0.5,1.,1.),//9
vec4(0.5,1.,1.,1.),//10


};


void main() {
	outColor = vec4(colorsPerLevel[colorID].xyz*dot(normal,-lightDir),1.);
	//outColor = vec4(normal,1.0);
}