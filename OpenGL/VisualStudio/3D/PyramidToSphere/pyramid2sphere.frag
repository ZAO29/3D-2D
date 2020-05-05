#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;


layout(binding =0) uniform sampler2D tex0;

layout (location = 0) in vec3 color;
layout (location = 1) in vec2 uv;

uniform float utessLevel;

void main() {

	
	vec3 texcolor = texture(tex0,uv).xyz;
	float t = exp((1.-utessLevel)/10.);
	outColor = vec4(t*color+(1.-t)*texcolor,1.0);
	//vec2 uv_mod = mod(uv,vec2(1.));
	//outColor = vec4(uv.x,0.,0.,1.);
}