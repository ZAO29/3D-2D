#version 450
#extension GL_ARB_separate_shader_objects : enable
layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;



layout(location = 0) in vec4 fragColor_in[];
layout(location = 0) out vec4 fragColor_out;

vec4 colorR = vec4(1.,1.,0.5,1);

vec4 colorG = vec4(0.5,1.,1.,1);

vec4 colorB = vec4(1.,0.5,1.,1);

vec4 colorW = vec4(1.,1.,1.,1);

uniform float uTime;

void main() {

	float coeff = mod(uTime,1.0);
	//coeff = 0.5f;
    vec4 middle01 = coeff*gl_in[0].gl_Position + (1.f-coeff)*gl_in[1].gl_Position;
    vec4 middle12 = coeff*gl_in[1].gl_Position + (1.f - coeff)*gl_in[2].gl_Position;
    vec4 middle20 = coeff*gl_in[2].gl_Position + (1.f-coeff)*gl_in[0].gl_Position;


    fragColor_out =(fragColor_in[0]+colorR)/2.;    
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();

    gl_Position = middle01;
    EmitVertex();

    gl_Position = middle20;
    EmitVertex();
    EndPrimitive();    


    fragColor_out =(fragColor_in[1]+colorB)/2.;    
    gl_Position = gl_in[1].gl_Position; 
    EmitVertex();

    gl_Position = middle01;
    EmitVertex();

    gl_Position = middle12;
    EmitVertex();
    EndPrimitive();

    fragColor_out =(fragColor_in[2]+colorG)/2.;    
    gl_Position = gl_in[2].gl_Position; 
    EmitVertex();

    gl_Position = middle20;
    EmitVertex();

    gl_Position = middle12;
    EmitVertex();
    EndPrimitive();


    fragColor_out =colorW;      
    gl_Position = middle01; 
    EmitVertex();
   
    gl_Position = middle20;
    EmitVertex();
   
    gl_Position = middle12;
    EmitVertex();
    EndPrimitive(); 


} 

