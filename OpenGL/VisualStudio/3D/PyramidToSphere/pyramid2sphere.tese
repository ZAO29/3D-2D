#version 450
#extension GL_ARB_separate_shader_objects : enable                                                                              
                                                                                                
layout(triangles, equal_spacing, ccw) in;                                                       
                                                                                                
uniform mat4 uMVP;                                                                               
                                                                   
                                                                                                
layout (location = 0) in vec3 pos_in[];                                                                       
layout (location = 1) in float colorID_in[];                                                                       
                                                                       
                                                                                                
layout (location = 0) out vec3 color_out;                                                                        
                                                                         
                                                                                                
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
    
	
	vec3 pos = gl_TessCoord.x* pos_in[0] + gl_TessCoord.y* pos_in[1] + gl_TessCoord.z* pos_in[2];  

    pos = normalize(pos);
	float P = atan(pos.z,length(pos.xy));
	float H = atan(pos.y,pos.x);
	// Set the control points of the output patch   
	vec3 x1=vec3(1.,0.,0.);
	vec3 x2=vec3(0.,1.,0.);
	vec3 x3=vec3(0.,0.,1.);	

	vec3 dirProj = cos(H)*x1+sin(H)*x2; 
	vec3 posSphere = cos(P)*dirProj+sin(P)*x3;


	
	gl_Position = uMVP * vec4(posSphere, 1.0);
	
	vec3 colors[4] = vec3[4](vec3(1.,1.,0.), vec3(1.,0.,1.), vec3(0.,1.,1.), vec3(1.,1.,1.));
	
	color_out= gl_TessCoord/4. + 3.*colors[int(colorID_in[0])]/4.;
}                                                                                               
