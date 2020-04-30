#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec2 uv_out;

layout (location = 0) in vec3 position;


uniform vec2 u_scale;




void main() {
     
    gl_Position = vec4(position, 1.0);
	uv_out = vec2(position.x*u_scale.x, position.y*u_scale.y);
}