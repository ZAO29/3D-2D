#version 450
#extension GL_ARB_separate_shader_objects : enable


uniform mat4 uModel;
uniform vec3 uCamPos;

// define the number of CPs in the output patch                                                 
layout (vertices = 3) out;                                                                      
                                                                                                
                                                                     
                                                                                                
// attributes of the input CPs                                                                  
layout (location = 0) in vec3 pos_in[];
layout (location = 1) in vec3 normal_in[];   
                                                                    
                                                                         
                                                                                                
// attributes of the output CPs
layout (location = 0) out vec3 pos_out[];
layout (location = 1) out vec3 normal_out[];  

                                                                    
uniform float utessLevel;                                                                                                                                                                                               
                                                                                               
void main()                                                                                     
{              
 
    pos_out[gl_InvocationID] = pos_in[gl_InvocationID];
	normal_out[gl_InvocationID] = normal_in[gl_InvocationID];
                    
	vec3 posWorld = (uModel * vec4(pos_in[gl_InvocationID],1.0)).xyz;	
	
	float d = distance(posWorld,uCamPos);

    // Calculate the tessellation levels                                                        
    gl_TessLevelOuter[0] = utessLevel/(1.+d);      
    gl_TessLevelOuter[1] = utessLevel/(1.+d);         
    gl_TessLevelOuter[2] = utessLevel/(1.+d);           
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];                                                
}                                                                                               