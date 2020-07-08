#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;


layout(binding =0) uniform sampler2D tex0;

layout (location = 0) in flat int colorID;
//layout (location = 1) in vec2 uv;

uniform float utessLevel;

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

	
	//vec3 texcolor = texture(tex0,uv).xyz;
	float t = exp((1.-utessLevel)/10.);
	outColor = colorsPerLevel[colorID];
	//vec2 uv_mod = mod(uv,vec2(1.));
	//outColor = vec4(uv.x,0.,0.,1.);
}