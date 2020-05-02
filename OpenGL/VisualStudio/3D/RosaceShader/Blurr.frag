#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;


layout(location = 0) in vec2 uv_out;
layout(binding =0) uniform sampler2D tex0;

uniform vec2 u_dir;
uniform vec2 u_resolution;
uniform vec2 u_powmult;


vec4 blurr(sampler2D image,vec2 uv,vec2 direction,vec2 resolution)
{
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += texture(image, uv) * 0.1964825501511404;
  color += texture(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}


void main() {

	outColor = vec4(blurr(tex0,uv_out,u_dir,u_resolution).xyz,1.0);
	outColor = pow(outColor,vec4(u_powmult.x));
	//outColor = smoothstep(vec4(0.),vec4(1.),outColor);
	outColor = outColor * u_powmult.y;
	outColor.w = 1.0;

}