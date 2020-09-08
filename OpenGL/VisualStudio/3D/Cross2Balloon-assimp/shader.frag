#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in float z;


layout(binding =0) uniform sampler2D tex;

void main() {

	outColor = texture(tex,vec2(0.,0.));

}