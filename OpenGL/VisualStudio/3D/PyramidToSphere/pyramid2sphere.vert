#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec3 color;

layout (location = 0) in vec2 HeadingPitch;




uniform mat4 uMVP;





void main() {

	 vec3 x1=vec3(1.,0.,0.);
	 vec3 x2=vec3(0.,1.,0.);
	 vec3 x3=vec3(0.,0.,1.);	

     float H = HeadingPitch.x;
	 float P = HeadingPitch.y;
	 vec3 dirProj = cos(H)*x1+sin(H)*x2; 
	 vec3 pos = cos(P)*dirProj+sin(P)*x3;
	 vec3 color = pos;
    gl_Position = uMVP * vec4(pos, 1.0); 
}