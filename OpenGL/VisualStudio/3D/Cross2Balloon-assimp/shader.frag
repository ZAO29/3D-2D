#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in float z;




void main() {

	outColor = vec4(z,z,z,z);
	outColor = vec4(0.,0.,1.,1.);
}