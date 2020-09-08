#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec2 HeadingPitch;

layout (location = 0) out vec3 pos;







uniform mat4 uMVP;

	 vec3 x1=vec3(1.,0.,0.);
	 vec3 x2=vec3(0.,1.,0.);
	 vec3 x3=vec3(0.,0.,1.);

void main() {
     float H = HeadingPitch.x;
	 float P = HeadingPitch.y;
	 vec3 dirProj = cos(H)*x1+sin(H)*x2; 
	 pos = cos(P)*dirProj+sin(P)*x3;
    gl_Position = uMVP * vec4(5.*pos, 1.0);
}