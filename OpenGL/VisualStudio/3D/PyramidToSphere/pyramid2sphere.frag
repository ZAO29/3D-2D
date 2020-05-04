#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;


layout(location = 0) in flat float colorID;


void main() {

	vec3 colors[4] = vec3[4](vec3(1.,1.,0.), vec3(1.,0.,1.), vec3(0.,1.,1.), vec3(1.,1.,1.));

	outColor = vec4(colors[int(colorID)],1.0);
}