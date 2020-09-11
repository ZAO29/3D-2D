#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 1) out vec3 posWorld;

layout(location = 0) out vec4 outColor;


uniform vec3 uCamPos;

vec3 normal = vec3(0.,1.,0.);

vec3 color = vec3(0.5); 
void main() {
	vec3 pos2Cam  = posWorld - uCamPos;
	vec3 reflection = normalize(reflect(pos2Cam,normal));
	
	outColor = vec4(abs(reflection)/2.+color/2., 1.);
}