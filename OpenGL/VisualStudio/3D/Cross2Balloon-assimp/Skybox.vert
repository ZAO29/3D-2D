#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec2 HeadingPitch;

layout (location = 0) out vec2 hpOut;







uniform mat4 uMVP;

	 vec3 x1=vec3(1.,0.,0.);
	 vec3 x2=vec3(0.,1.,0.);
	 vec3 x3=vec3(0.,0.,1.);

void main() {
	hpOut = HeadingPitch;
    //gl_Position = uMVP * vec4(5.*pos, 1.0);
}