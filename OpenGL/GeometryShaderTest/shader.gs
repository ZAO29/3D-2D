#version 460 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;



in VS_OUT {
    vec2 uv;
} gs_in[];

out VS_OUT {
    vec2 uv;
} vs_out;

void main() {
        vs_out.uv = gs_in[0].uv;    
        gl_Position = gl_in[0].gl_Position + vec4(-0.3, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.3, 0.0, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
} 

