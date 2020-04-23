#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 uv_out;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in float alpha;



uniform mat4 uMVP;


void main() {
     
    gl_Position = uMVP * vec4(position, 1.0);
	//gl_Position = vec4(usize * position, 1.0);
	fragColor = vec4(vec3(0.),alpha);
	uv_out = uv; 
}