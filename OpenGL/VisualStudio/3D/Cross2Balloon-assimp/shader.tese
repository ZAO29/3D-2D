#version 450
#extension GL_ARB_separate_shader_objects : enable                                                                              
                                                                                                
layout(triangles, equal_spacing, ccw) in;                                                       
                                                                                                
                                                                               
                                                                   
                                                                                                
layout (location = 0) in vec3 pos_in[];
layout (location = 1) in vec3 normal_in[];  
                                                                       
                                                                                                
layout(location = 0) out float z;
layout(location = 1) out vec3 normalWorld;
layout(location = 2) out vec3 posWorld;

                                                                         
                                                                                                
                                                                                          
uniform mat4 uMVP;
uniform mat4 uModel;
uniform vec4 uCenterRadius;
uniform float utessLevel;
uniform float utessLevelMax;


float PI = 3.14159265359f;  
                                                                                                
void main()                                                                                     
{                                                                                               
    vec3 center = uCenterRadius.xyz;
	float radius = uCenterRadius.w;
	
	vec3 pos = gl_TessCoord.x* pos_in[0] + gl_TessCoord.y* pos_in[1] + gl_TessCoord.z* pos_in[2];
	vec3 normal = gl_TessCoord.x* normal_in[0] + gl_TessCoord.y* normal_in[1] + gl_TessCoord.z* normal_in[2];
	normal = normalize(normal);

	vec3 posSphere = pos - center;
	vec3 normalSphere = normalize(posSphere);
	posSphere = normalSphere*radius + center;
	
	//float t = exp((1.-utessLevel)/10.);
	float t = 1.-(utessLevel-1.)/(utessLevelMax-1.);
	//float t = utessLevel/100.;
	//float t = 1.0;
	vec3 Posmean = (1.-t)*posSphere+t*pos;   
	vec3 Normalmean = normalize((1.-t)*normalSphere+t*normal);
  

	pos = Posmean;
	normal = Normalmean;
	gl_Position = uMVP * vec4(pos, 1.0);
	normalWorld = normalize((uModel * vec4(normal, 1.0)).xyz);
	posWorld = (uModel * vec4(pos, 1.0)).xyz;
	z = 0;


}                                                                                               
