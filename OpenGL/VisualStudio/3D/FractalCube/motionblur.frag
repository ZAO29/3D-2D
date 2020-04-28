#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;


layout(location = 0) in vec2 uv_out;
layout(binding =0) uniform sampler2D tex0;
layout(binding =1) uniform sampler2D tex1;
layout(binding =2) uniform sampler2D tex2;
layout(binding =3) uniform sampler2D tex3;
layout(binding =4) uniform sampler2D tex4;
layout(binding =5) uniform sampler2D tex5;
layout(binding =6) uniform sampler2D tex6;
layout(binding =7) uniform sampler2D tex7;
layout(binding =8) uniform sampler2D tex8;
layout(binding =9) uniform sampler2D tex9;
layout(binding =10) uniform sampler2D tex10;



vec2 offset1 = vec2(0.002,0.002);

void main() {
	
	vec2 offset2 = vec2(offset1.y,-offset1.x);

	vec3 color0 = texture(tex0,uv_out+offset1).xyz;
	vec3 color1 = texture(tex1,uv_out+offset2).xyz;
	vec3 color2 = texture(tex2,uv_out-offset1).xyz;
	vec3 color3 = texture(tex3,uv_out-offset2).xyz;
	vec3 color4 = texture(tex4,uv_out).xyz;
	vec3 color5 = texture(tex5,uv_out).xyz;
	vec3 color6 = texture(tex6,uv_out).xyz;
	vec3 color7 = texture(tex7,uv_out).xyz;
	vec3 color8 = texture(tex8,uv_out).xyz;
	vec3 color9 = texture(tex9,uv_out).xyz;
	vec3 color10 = texture(tex10,uv_out).xyz;

	outColor = vec4((color0 + color1 + color2 + color3 + color4)/5.,1.0);

	vec4 diff= vec4(color0-color4,1.);

	outColor = abs(diff + outColor)/2.;
	//outColor = diff;
}