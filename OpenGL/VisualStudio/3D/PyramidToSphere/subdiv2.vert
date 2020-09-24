#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 pos;
layout (location = 1) in float colorID;


layout(location = 0) out flat int colorID_out;



uniform mat4 uMVP;





void main() {

	 colorID_out = int(floor(colorID));
    gl_Position = vec4(pos, 1.0); 
}