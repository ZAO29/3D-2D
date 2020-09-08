#version 450
#extension GL_ARB_separate_shader_objects : enable

// define the number of CPs in the output patch                                                 
layout (vertices = 3) out;                                                                      
                                                                                                
                                                                     
                                                                                                
// attributes of the input CPs                                                                  
layout (location = 0) in vec2 HP_in[];                                                                       
                                                                    
                                                                         
                                                                                                
// attributes of the output CPs
layout (location = 0) out vec3 pos_out[];                                                                       

                                                                    
uniform float utessLevel;                                                                                                                                                                                               
                                                                                               
void main()                                                                                     
{              

	vec3 x1=vec3(1.,0.,0.);
	vec3 x2=vec3(0.,1.,0.);
	vec3 x3=vec3(0.,0.,1.);	
	float H = HP_in[gl_InvocationID].x;
	float P = HP_in[gl_InvocationID].y;
	vec3 dirProj = cos(H)*x1+sin(H)*x2; 
	vec3 posSphere = cos(P)*dirProj+sin(P)*x3;
    
    pos_out[gl_InvocationID] = posSphere;                          
                                                                                                                                                                         
    // Calculate the tessellation levels                                                        
    gl_TessLevelOuter[0] = utessLevel;      
    gl_TessLevelOuter[1] = utessLevel;         
    gl_TessLevelOuter[2] = utessLevel;           
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];                                                
}                                                                                               