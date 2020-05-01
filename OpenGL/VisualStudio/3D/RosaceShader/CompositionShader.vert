#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec2 uv_out;

layout (location = 0) in vec3 position;







void main() {
     
    gl_Position = vec4(position, 1.0);
	uv_out = (position.xy + 1.)/2.;
}