#version 450
#extension GL_ARB_separate_shader_objects : enable                                                                              
                                                                                                
layout(triangles, equal_spacing, ccw) in;                                                       
                                                                                                
uniform mat4 uMVP;                                                                               
                                                                   
                                                                                                
layout (location = 0) in vec3 pos_in[];                                                                       
layout (location = 1) in flat float colorID_in[];                                                                       
                                                                       
                                                                                                
layout (location = 0) out vec3 color_out;                                                                        
                                                                         
                                                                                                
uniform float utessLevel;                                                                                           
                                                                                                
void main()                                                                                     
{                                                                                               
    
	
	vec3 pos = gl_TessCoord.x* pos_in[0] + gl_TessCoord.y* pos_in[1] + gl_TessCoord.z* pos_in[2];  

    vec3 posN = normalize(pos);
	float P = atan(posN.z,length(posN.xy));
	float H = atan(posN.y,posN.x);
	// Set the control points of the output patch   
	vec3 x1=vec3(1.,0.,0.);
	vec3 x2=vec3(0.,1.,0.);
	vec3 x3=vec3(0.,0.,1.);	

	vec3 dirProj = cos(H)*x1+sin(H)*x2; 
	vec3 posSphere = cos(P)*dirProj+sin(P)*x3;
	float t = exp((1.-utessLevel)/10.);
	//float t = utessLevel/100.;
	//float t = 1.0;
	vec3 Posmean = (1.-t)*posSphere+t*pos;   

	
	gl_Position = uMVP * vec4(Posmean, 1.0);
	
	vec3 colors[4] = vec3[4](vec3(1.,1.,0.), vec3(1.,0.,1.), vec3(0.,1.,1.), vec3(1.,1.,1.));
	
	color_out= colors[int(colorID_in[0])];
}                                                                                               
