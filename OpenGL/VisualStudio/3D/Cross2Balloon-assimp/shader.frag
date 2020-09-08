#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in float z;
layout(location = 1) in vec3 normalWorld;
layout(location = 2) in vec3 posWorld;


layout(binding =0) uniform sampler2D tex;

uniform mat4 uView;
uniform vec3 uCamPos;
uniform float uSpecularPow;
uniform float uSpecularIntensity;



vec3 lightDir = vec3(1.,1.,1.);
void main() {
	float diffuse = max(0.,dot(lightDir,-normalize(normalWorld)));
	vec3 reflection = reflect(lightDir,normalize(normalWorld));
	vec3 pos2Cam  = uCamPos - posWorld;
	float specular = uSpecularIntensity * pow(max(0.,dot(pos2Cam,reflection)),uSpecularPow);
	outColor = diffuse*texture(tex,vec2(0.,0.)) + specular * vec4(1.,1.,1.,1.);

}