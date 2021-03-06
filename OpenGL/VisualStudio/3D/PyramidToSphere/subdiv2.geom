#version 450
#extension GL_ARB_separate_shader_objects : enable



layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out; //WARNING HARDWARE LIMITATION reached, can only emit n vertices of this size




layout(location = 0) in flat int colorID[];

layout(location = 0) out flat int colorID_out;
layout(location = 1) out flat vec3  normal_out;

uniform mat4 uMVP;

uniform float uMeanMiddle;
uniform float uMeanSphere;
uniform int uLevel;

void generateTri(vec3 pos1, vec3 pos2, vec3 pos3, int colorId, float coeff)
{
	vec3 rpos1 = coeff* 3.*normalize(pos1) + (1.-coeff) * pos1;
	vec3 rpos2 = coeff* 3.*normalize(pos2) + (1.-coeff) * pos2;
	vec3 rpos3 = coeff* 3.*normalize(pos3) + (1.-coeff) * pos3;
	
	vec3 v1 = rpos2 - rpos1;
	vec3 v2 = rpos3 - rpos1;

	vec3 n = normalize(cross(v1,v2));
	if(dot(rpos1,n) < 0)
	{	
		n = -n;
	}

	gl_Position = uMVP*vec4(rpos1,1.0); 
	colorID_out = colorId;
	normal_out = n;
    EmitVertex();

    gl_Position = uMVP*vec4(rpos2,1.0); 
	normal_out = n;
    EmitVertex();

    gl_Position =  uMVP*vec4(rpos3,1.0);
	normal_out = n;
    EmitVertex();
    EndPrimitive();    
}




void main() {

	vec3 middles[3];
	float coeff = uMeanMiddle;
			
	for(int i=0;i<3;i++)
	{
		int j = i+1;
		if(j==3)
		{
			j = 0;
		}
		middles[i] = coeff*gl_in[i].gl_Position.xyz + (1.f-coeff)*gl_in[j].gl_Position.xyz;
	
	}

	generateTri(middles[0], middles[1], middles[2], colorID[0], uMeanSphere);
	generateTri(middles[0], middles[1], gl_in[1].gl_Position.xyz, uLevel+1, uMeanSphere);
	generateTri(middles[1], middles[2], gl_in[2].gl_Position.xyz, uLevel+1, uMeanSphere);
	generateTri(middles[0], middles[2], gl_in[0].gl_Position.xyz, uLevel+1, uMeanSphere);
	


} 

