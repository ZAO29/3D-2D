#version 450
#extension GL_ARB_separate_shader_objects : enable                                                                              
                                                                                                
layout(triangles, equal_spacing, ccw) in;                                                       
                                                                                                
uniform mat4 uMVP;                                                                               
                                                                   
                                                                                                
layout (location = 0) in vec2 HP_in[];                                                                       
layout (location = 1) in float colorID_in[];                                                                       
                                                                       
                                                                                                
layout (location = 0) out flat float colorID_out;                                                                        
                                                                         
                                                                                                
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)                                                   
{                                                                                               
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;   
}                                                                                               
                                                                                                
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)                                                   
{                                                                                               
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;   
}                                                                                               
                                                                                                
void main()                                                                                     
{                                                                                               
    vec2 HP = gl_TessCoord.x* HP_in[0] + gl_TessCoord.y* HP_in[1] + gl_TessCoord.z* HP_in[2];  
	vec3 x1=vec3(1.,0.,0.);
	vec3 x2=vec3(0.,1.,0.);
	vec3 x3=vec3(0.,0.,1.);	

    float H = HP.x;
	float P = HP.y;
	vec3 dirProj = cos(H)*x1+sin(H)*x2; 
	vec3 pos = cos(P)*dirProj+sin(P)*x3;
    gl_Position = uMVP * vec4(pos, 1.0);
	colorID_out=colorID_in[0];
}                                                                                               
