#version 450
#extension GL_ARB_separate_shader_objects : enable                                                                              
                                                                                                
layout(triangles, equal_spacing, ccw) in;                                                       
                                                                                                
                                                                               
                                                                   
                                                                                                
layout (location = 0) in vec3 pos_in[];                                                                                                                                              
                                                                       
                                                                                                
layout (location = 0) out vec3 color_out;


                                                                         
                                                                                                
                                                                                          
uniform mat4 uMVP;

float PI = 3.14159265359f;  
                                                                                                
void main()                                                                                     
{                                                                                               
    
	
	vec3 pos = gl_TessCoord.x* pos_in[0] + gl_TessCoord.y* pos_in[1] + gl_TessCoord.z* pos_in[2];  

	pos = normalize(pos);
	
  

	
	gl_Position = uMVP * vec4(pos, 1.0);

	
	color_out= pos;

}                                                                                               
