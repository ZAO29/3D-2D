#version 450
#extension GL_ARB_separate_shader_objects : enable

// define the number of CPs in the output patch                                                 
layout (vertices = 3) out;                                                                      
                                                                                                
uniform vec3 gEyeWorldPos;                                                                      
                                                                                                
// attributes of the input CPs                                                                  
layout (location = 0) in vec2 HP_in[];                                                                       
layout (location = 1) in float colorID_in[];                                                                       
                                                                         
                                                                                                
// attributes of the output CPs
layout (location = 0) out vec3 pos_out[];                                                                       
layout (location = 1) out float colorID_out[];  
                                                                    
uniform float utessLevel;                                                                                                                                                                                               
                                                                                               
void main()                                                                                     
{                                                                                               
    // Set the control points of the output patch   
	vec3 x1=vec3(1.,0.,0.);
	vec3 x2=vec3(0.,1.,0.);
	vec3 x3=vec3(0.,0.,1.);	

    float H = HP_in[gl_InvocationID].x;
	float P = HP_in[gl_InvocationID].y;
	vec3 dirProj = cos(H)*x1+sin(H)*x2; 
	vec3 pos = cos(P)*dirProj+sin(P)*x3;
    pos_out[gl_InvocationID] = pos;                          
    colorID_out[gl_InvocationID] = colorID_in[gl_InvocationID];                    
                                                                                                                                                                                                
    // Calculate the tessellation levels                                                        
    gl_TessLevelOuter[0] = utessLevel;      
    gl_TessLevelOuter[1] = utessLevel;         
    gl_TessLevelOuter[2] = utessLevel;           
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];                                                
}                                                                                               