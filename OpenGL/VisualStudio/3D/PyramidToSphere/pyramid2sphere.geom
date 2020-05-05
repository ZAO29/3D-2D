#version 450
#extension GL_ARB_separate_shader_objects : enable

struct Data
{
	vec4 pos;
	vec2 uv;
};

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;



layout(location = 0) in flat vec3 fragColor_in[];
layout(location = 1) in vec2 uv_in[];
layout(location = 0) out vec3 fragColor_out;
layout(location = 1) out vec2 uv_out;







vec4 colorR = vec4(0.,0.,0.,1);

vec4 colorG = vec4(0.,0.,0.,1);

vec4 colorB = vec4(0.,0.,0.,1);

vec4 colorW = vec4(1.,1.,1.,1);

//uniform float uTime;
//uniform vec4 uMultiUse;
uniform float umult;





void generateTri(Data vert1,Data vert2,Data vert3)
{
	gl_Position = vert1.pos; 
	uv_out = vert1.uv;
    EmitVertex();

    gl_Position = vert2.pos;
	uv_out = vert2.uv;
    EmitVertex();

    gl_Position = vert3.pos;
	uv_out = vert3.uv;
    EmitVertex();
    EndPrimitive();    
}



void main() {

	Data middle01,middle12,middle20,pt0,pt1,pt2;
	//vec4 bidon = uMultiUse;
	//float coeff = mod(uMultiUse.w*mod(uMultiUse.x*uTime+uMultiUse.y*uv_in[0].x+uMultiUse.z*uv_in[0].y,1.0),1.0);
	float coeff = 0.5f;
    middle01.pos = coeff*gl_in[0].gl_Position + (1.f-coeff)*gl_in[1].gl_Position;
    middle12.pos = coeff*gl_in[1].gl_Position + (1.f - coeff)*gl_in[2].gl_Position;
    middle20.pos = coeff*gl_in[2].gl_Position + (1.f-coeff)*gl_in[0].gl_Position;

	middle01.uv = coeff*uv_in[0] + (1.f-coeff)*uv_in[1];
    middle12.uv = coeff*uv_in[1] + (1.f - coeff)*uv_in[2];
    middle20.uv = coeff*uv_in[2] + (1.f-coeff)*uv_in[0];

	pt0.pos = gl_in[0].gl_Position;
	pt0.uv = uv_in[0];
	pt1.pos = gl_in[1].gl_Position;
	pt1.uv = uv_in[1];
	pt2.pos = gl_in[2].gl_Position;
	pt2.uv = uv_in[2];
    fragColor_out =(fragColor_in[0])*umult; 
	generateTri(pt0,middle01,middle20);
 


    fragColor_out =(fragColor_in[1])*umult;
	generateTri(pt1,middle01,middle12);
 

    fragColor_out =(fragColor_in[2])*umult;
	generateTri(pt2,middle20,middle12);


    fragColor_out =fragColor_in[0];      
    generateTri(middle01,middle20,middle12);


} 

