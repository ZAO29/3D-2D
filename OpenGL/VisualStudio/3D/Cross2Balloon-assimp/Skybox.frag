#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 pos;

layout(location = 0) out vec4 outColor;








vec3 lightDir = vec3(1.,1.,1.);
void main() {
	
	outColor = vec4(pos,1.);

}