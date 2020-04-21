#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;



in VS_OUT {
    vec2 uv;
} gs_in[];

out VS_OUT {
    vec2 uv;
} vs_out;

void main() {

    vec4 middle01 = (gl_in[0].gl_Position + gl_in[1].gl_Position)/2.;
    vec4 middle12 = (gl_in[1].gl_Position + gl_in[2].gl_Position)/2.;
    vec4 middle20 = (gl_in[2].gl_Position + gl_in[0].gl_Position)/2.;


    vs_out.uv = gs_in[0].uv;    
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();

    gl_Position = middle01;
    EmitVertex();

    gl_Position = middle20;
    EmitVertex();
    EndPrimitive();    


    vs_out.uv = gs_in[1].uv;    
    gl_Position = gl_in[1].gl_Position; 
    EmitVertex();

    gl_Position = middle01;
    EmitVertex();

    gl_Position = middle12;
    EmitVertex();
    EndPrimitive();

    vs_out.uv = gs_in[2].uv;    
    gl_Position = gl_in[2].gl_Position; 
    EmitVertex();

    gl_Position = middle20;
    EmitVertex();

    gl_Position = middle12;
    EmitVertex();
    EndPrimitive();


    vs_out.uv = gs_in[0].uv;    
    gl_Position = middle01; 
    EmitVertex();
    vs_out.uv = gs_in[2].uv;
    gl_Position = middle20;
    EmitVertex();
    vs_out.uv = gs_in[1].uv;
    gl_Position = middle12;
    EmitVertex();
    EndPrimitive(); 


} 

