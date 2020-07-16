#version 450
#extension GL_ARB_separate_shader_objects : enable

struct Data
{
	vec4 pos;
	vec2 uv;
};

struct TriData
{
	Data pts[3];
	int colorID;
	int level;
	
};

layout (triangles) in;
layout (triangle_strip, max_vertices = 204) out; //WARNING HARDWARE LIMITATION reached, can only emit n vertices of this size



layout(location = 0) in flat vec3 fragColor_in[];
layout(location = 1) in vec2 uv_in[];
layout(location = 0) out flat int fragColor_out;
//layout(location = 1) out vec2 uv_out;







vec4 colorR = vec4(0.,0.,0.,1);

vec4 colorG = vec4(0.,0.,0.,1);

vec4 colorB = vec4(0.,0.,0.,1);

vec4 colorW = vec4(1.,1.,1.,1);


uniform float usubdiv;
uniform mat4 uMVP;









void generateTri(Data vert1,Data vert2,Data vert3);
void generateTri(Data vert1,Data vert2,Data vert3)
{
	gl_Position = vert1.pos; 
	//uv_out = vert1.uv;
    EmitVertex();

    gl_Position = vert2.pos;
	//uv_out = vert2.uv;
    EmitVertex();

    gl_Position = vert3.pos;
	//uv_out = vert3.uv;
    EmitVertex();
    EndPrimitive();    
}


void generateTri(TriData tri);
void generateTri(TriData tri)
{
	fragColor_out = tri.colorID;
	generateTri(tri.pts[0],tri.pts[1],tri.pts[2]);
}



int getSumOf4(int i)
{
	return int(pow(4,float(i))-1)/3;
}

void Subdivide(float level,Data tri[3])
{
	TriData listTri[500];
	TriData dataBegin;
	float coeff = 0.5f;
	for(int i=0;i<3;i++)
	{
		dataBegin.pts[i]=tri[i];
	}
	dataBegin.level = 0;
	dataBegin.colorID = 0;
	//generateTri(listTri[0]);

	TriData TriData1,TriData2,TriData3,TriData4; 

	listTri[0] = dataBegin;

	int ceillevel = int(ceil(level));
	float coefFinal = (level - floor(level))/2.;
	if(float(ceillevel) == level)
	{
	coefFinal = 0.5;
	}

	for (int currentLevel=0;currentLevel<ceillevel;currentLevel++)
	{
		int kdep = getSumOf4(currentLevel);
		int kfin = getSumOf4(currentLevel+1);

		if(currentLevel == ceillevel - 1)
		{
			coeff = coefFinal;
		}

		TriData TriDataToSub;

		for (int k=kdep;k<kfin;k++)
		{
			TriDataToSub = listTri[k];	
			Data middles[3];

			
			for(int i=0;i<3;i++)
			{
				int j = i+1;
				if(j==3)
				{
					j = 0;
				}
				middles[i].pos = coeff*TriDataToSub.pts[i].pos + (1.f-coeff)*TriDataToSub.pts[j].pos;
				middles[i].uv = coeff*TriDataToSub.pts[i].uv + (1.f-coeff)*TriDataToSub.pts[j].uv;
			}
   
			

		   TriData1.pts[0] = TriDataToSub.pts[0];
		   TriData1.pts[1] = middles[0];
		   TriData1.pts[2] = middles[2];
		   TriData1.level = currentLevel;
		   TriData1.colorID = 3*currentLevel+1;
		   //TriData1.colorID = 0;
		   listTri[kfin+(k-kdep)*4]=TriData1;

		   TriData2.pts[0] = TriDataToSub.pts[1];
		   TriData2.pts[1] = middles[0];
		   TriData2.pts[2] = middles[1];
		   TriData2.level = currentLevel;
		   TriData2.colorID = 3*currentLevel+2;
		   //TriData2.colorID = 1;
		   listTri[kfin+(k-kdep)*4+1]=TriData2;
 
		   TriData3.pts[0] = TriDataToSub.pts[2];
		   TriData3.pts[1] = middles[2];
		   TriData3.pts[2] = middles[1];
		   TriData3.level = currentLevel;
		   TriData3.colorID = 3*currentLevel+3;
		   //TriData3.colorID = 2;
		   listTri[kfin+(k-kdep)*4+2]=TriData3;

		   TriData4.pts[0] = middles[0];
		   TriData4.pts[1] = middles[1];
		   TriData4.pts[2] = middles[2];
		   TriData4.level = currentLevel;
		   TriData4.colorID = TriDataToSub.colorID;
		   //TriData4.colorID = 3;
		   listTri[kfin+(k-kdep)*4+3]=TriData4;

		   //if(currentLevel == (level-1))
		   //{
				//generateTri(TriData1);
				//generateTri(TriData2);
				//generateTri(TriData3);
				//generateTri(TriData4);
		   //}

		}



	}

		for (int k=getSumOf4(ceillevel);k< getSumOf4(ceillevel+1);k++)
		{
			generateTri(listTri[k]);
		}
}






void main() {


	Data pts[3];
	for(int i=0;i<3;i++)
	{
		pts[i].pos = uMVP * gl_in[i].gl_Position;
		pts[i].uv = uv_in[i];
    
	}


	Subdivide(usubdiv,pts);
	
	//float coeff = 0.2f;
    //middle01.pos = coeff*gl_in[0].gl_Position + (1.f-coeff)*gl_in[1].gl_Position;
    //middle12.pos = coeff*gl_in[1].gl_Position + (1.f - coeff)*gl_in[2].gl_Position;
    //middle20.pos = coeff*gl_in[2].gl_Position + (1.f-coeff)*gl_in[0].gl_Position;

	//middle01.uv = coeff*uv_in[0] + (1.f-coeff)*uv_in[1];
    //middle12.uv = coeff*uv_in[1] + (1.f - coeff)*uv_in[2];
    //middle20.uv = coeff*uv_in[2] + (1.f-coeff)*uv_in[0];
	//
	//pt0.pos = gl_in[0].gl_Position;
	//pt0.uv = uv_in[0];
	//pt1.pos = gl_in[1].gl_Position;
	//pt1.uv = uv_in[1];
	//pt2.pos = gl_in[2].gl_Position;
	//pt2.uv = uv_in[2];
    //fragColor_out =(fragColor_in[0])*umult; 
	//generateTri(pt0,middle01,middle20);
 	//
	//
	//
    //fragColor_out =(fragColor_in[1])*umult;
	//generateTri(pt1,middle01,middle12);
 	//
	//
    //fragColor_out =(fragColor_in[2])*umult;
	//generateTri(pt2,middle20,middle12);
	//
	//
    //fragColor_out =fragColor_in[0];      
    //generateTri(middle01,middle20,middle12);


} 

