#version 450
#extension GL_ARB_separate_shader_objects : enable

// define the number of CPs in the output patch                                                 
layout (vertices = 3) out;                                                                      
                                                                                                
uniform vec3 gEyeWorldPos;                                                                      
                                                                                                
// attributes of the input CPs                                                                  
layout (location = 0) in vec2 HP_in[];                                                                       
layout (location = 1) in float colorID_in[];                                                                       
                                                                         
                                                                                                
// attributes of the output CPs
layout (location = 0) out vec2 HP_out[];                                                                       
layout (location = 1) out float colorID_out[];  
                                                                    
uniform float utessLevel;                                                                                                                                                                                               
                                                                                               
void main()                                                                                     
{                                                                                               
    // Set the control points of the output patch                                               
    HP_out[gl_InvocationID] = HP_in[gl_InvocationID];                          
    colorID_out[gl_InvocationID] = colorID_in[gl_InvocationID];                    
                                                                                                                                                                                                
    // Calculate the tessellation levels                                                        
    gl_TessLevelOuter[0] = utessLevel;      
    gl_TessLevelOuter[1] = utessLevel;         
    gl_TessLevelOuter[2] = utessLevel;           
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];                                                
}                                                                                               