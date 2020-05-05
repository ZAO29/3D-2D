#version 450
#extension GL_ARB_separate_shader_objects : enable
layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;



layout(location = 0) in flat vec3 fragColor_in[];
layout(location = 0) out vec3 fragColor_out;

vec4 colorR = vec4(0.,0.,0.,1);

vec4 colorG = vec4(0.,0.,0.,1);

vec4 colorB = vec4(0.,0.,0.,1);

vec4 colorW = vec4(1.,1.,1.,1);

//uniform float uTime;
//uniform vec4 uMultiUse;
uniform float umult;
void generateTri(vec4 vert1, vec4 vert2, vec4 vert3)
{
	gl_Position = vert1; 
	
    EmitVertex();

    gl_Position = vert2;
	
    EmitVertex();

    gl_Position = vert3;
	
    EmitVertex();
    EndPrimitive();    
}



void main() {

	//vec4 bidon = uMultiUse;
	//float coeff = mod(uMultiUse.w*mod(uMultiUse.x*uTime+uMultiUse.y*uv_in[0].x+uMultiUse.z*uv_in[0].y,1.0),1.0);
	float coeff = 0.5f;
    vec4 middle01 = coeff*gl_in[0].gl_Position + (1.f-coeff)*gl_in[1].gl_Position;
    vec4 middle12 = coeff*gl_in[1].gl_Position + (1.f - coeff)*gl_in[2].gl_Position;
    vec4 middle20 = coeff*gl_in[2].gl_Position + (1.f-coeff)*gl_in[0].gl_Position;


    fragColor_out =(fragColor_in[0])*umult; 
	generateTri(gl_in[0].gl_Position,middle01,middle20);
 


    fragColor_out =(fragColor_in[1])*umult;
	generateTri(gl_in[1].gl_Position,middle01,middle12);
 

    fragColor_out =(fragColor_in[2])*umult;
	generateTri(gl_in[2].gl_Position,middle20,middle12);


    fragColor_out =fragColor_in[0];      
    generateTri(middle01,middle20,middle12);


} 

