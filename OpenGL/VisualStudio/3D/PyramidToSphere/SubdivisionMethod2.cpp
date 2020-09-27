#include "stdafx.h"
#include "SubdivisionMethod2.h"

#include <gtc/type_ptr.hpp>
#include <ZGL/imgui/imgui.h>


#define SHADER_COEFF "uMeanMiddle"

#define SHADER_COEFFSPHERE "uMeanSphere"

#define SHADER_LEVEL "uLevel"


SubdivisionMethod2::SubdivisionMethod2()
{
}


SubdivisionMethod2::~SubdivisionMethod2()
{
}

struct VertexMethod2Data
{
	VertexMethod2Data(glm::vec3 pos,int color) :m_pos(pos), m_colorId(color) {}
	VertexMethod2Data() :m_pos(0), m_colorId(0) {}

	glm::vec3 m_pos;
	float m_colorId = 0; // impossible to send int maybe because of paddling ?
};



std::vector<VertexMethod2Data> Subdiv(std::vector<VertexMethod2Data> & vertices, int level)
{
	size_t nbFace = vertices.size() / 3;
	std::vector<VertexMethod2Data> newVertices;
	newVertices.resize(vertices.size() * 4);

	

	for (int i = 0;i < nbFace; i++)
	{
		VertexMethod2Data v[3];
		VertexMethod2Data middles[3];
		int j1;
		for (int j = 0; j < 3; j++)
		{
			j1 = (j + 1) % 3;
			v[j] = vertices[3 * i + j];
			v[j].m_colorId = level;
			v[j1] = vertices[3 * i + j1];
			v[j1].m_colorId = level;
			middles[j].m_pos = (v[j].m_pos + v[j1].m_pos) / 2.0f;
			middles[j].m_colorId = vertices[3 * i + j].m_colorId;


		}

		newVertices[i * 12] = middles[0];
		newVertices[i * 12+1] = middles[1];
		newVertices[i * 12+2] = middles[2];


		middles[0].m_colorId = level;
		middles[1].m_colorId = level;
		middles[2].m_colorId = level;
	
		for (int k = 0; k < 3; k++)
		{
			newVertices[i * 12 + k*3+3] = middles[k];
			newVertices[i * 12 + k*3+4] = middles[(k+1)%3];
			newVertices[i * 12 + k*3+5] = v[(k + 1) % 3];
		}
		
	}
	return newVertices;
}


void SubdivisionMethod2::Init()
{
	float PitchBase = PI / 2. - acos(-1. / 3.);

	glm::vec2 hp0(0, PI / 2.);
	glm::vec2 hp1(0, PitchBase);
	glm::vec2 hp2(2.*PI / 3., PitchBase);
	glm::vec2 hp3(4.*PI / 3., PitchBase);


	void setPitchAndHeadingRad(glm::vec3 & dir, float  H, float  P);
	std::vector<glm::vec2> hps =
	{
		hp0, hp1, hp2,
		hp0, hp2, hp3,
		hp0, hp3, hp1,
		hp1, hp2, hp3,

	};


	std::vector<VertexMethod2Data> vertices;/* = { VertexMethod2Data(glm::vec3(0),0),
											   VertexMethod2Data(glm::vec3(0.,1.,0.),1),
											   VertexMethod2Data(glm::vec3(1.,0.,0.),2) }; */
	vertices.resize(hps.size());

	int i = 0;
	for (auto & hp : hps)
	{
		setPitchAndHeadingRad(vertices[i++].m_pos, hp.x, hp.y);
	}

	

	ZGLStride stride1, stride2;
	stride1.m_offset = sizeof(glm::vec3);
	stride1.m_type = GL_FLOAT;
	stride1.m_size = 3;

	stride2.m_offset = sizeof(float);
	stride2.m_type = GL_FLOAT;
	stride2.m_size = 1;

	m_levelPyramids.resize(m_levelMax);

	ZGLVAODrawableParam paramDrawable;
	paramDrawable.m_stride = sizeof(VertexMethod2Data);
	paramDrawable.m_strides = { stride1,stride2 };

	
	int level = 0;
	for (int i = 0; i < m_levelMax; i++)
	{
		paramDrawable.m_nbVertex = vertices.size();
		paramDrawable.m_pVertices = (void *)&vertices[0];
		m_levelPyramids[i].Init(paramDrawable);

		level++;
		vertices = Subdiv(vertices,level);
	}
	


	MapUniform uniformMap;
	uniformMap[SHADER_MVP] = eZGLtypeUniform::ZGL_FMAT4;
	uniformMap[SHADER_COEFF] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_COEFFSPHERE] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_LEVEL] = eZGLtypeUniform::ZGL_IVEC1;
	GraphicPipelineType type;
	type.geometry = true;
	m_shader.Init("subdiv2", uniformMap, type);


}

void SubdivisionMethod2::Render(Camera * pCam)
{
	m_shader.Enable();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glm::mat4 mvp = pCam->getProjectionAndView();
	m_shader.updateUniform(SHADER_MVP, glm::value_ptr(mvp));
	


	int id = int(std::floor(m_subdiv));
	float r = (m_subdiv - float(id))/2.0;
	m_shader.updateUniform(SHADER_COEFF, &r);
	float ratio = m_subdiv / m_levelMax;
	m_shader.updateUniform(SHADER_COEFFSPHERE, &ratio);
	m_shader.updateUniform(SHADER_LEVEL, &id);
	m_levelPyramids[id].Render(GL_TRIANGLES);

}

void SubdivisionMethod2::ImguiDraw()
{
	
	if (ImGui::SliderFloat("subdiv (manual tessellation) ", &m_subdiv, 0.0, m_levelMax - 0.01))
	{
		m_shader.Enable();
		
	}
}

void SubdivisionMethod2::Destroy()
{
	for (auto & level : m_levelPyramids)
	{
		level.Destroy();
	}
}
