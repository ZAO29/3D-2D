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
uniform vec3 uLightDir = vec3(1.,1.,1.);
uniform float uReflectWeight;

void main() {
	float diffuse = max(0.,dot(uLightDir,-normalWorld));
	vec3 reflection = reflect(uLightDir,normalWorld);
	vec3 pos2Cam  = uCamPos - posWorld;

	//vec4 mirror = vec4(abs(normalize(reflect(-pos2Cam,normalWorld))),1.0);

	vec4 mirror = vec4(normalize(reflect(pos2Cam,normalWorld)),1.0);
	vec4 colorDiffuse = texture(tex,vec2(0.,0.));

	float specular = uSpecularIntensity * pow(max(0.,dot(pos2Cam,reflection)),uSpecularPow);

	outColor = (colorDiffuse*(1.-uReflectWeight)+mirror*uReflectWeight) + specular * vec4(1.,1.,1.,1.);

}