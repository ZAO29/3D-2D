#include "stdafx.h"
#include "RosaceShaderApp.h"

#include <ZGL/imgui/imgui.h>

#define SHADER_TIME "u_time"
#define SHADER_SCALE "u_scale"
#define SHADER_NB_PT "u_nbPt"

RosaceShaderApp::RosaceShaderApp()
{
}


RosaceShaderApp::~RosaceShaderApp()
{
}

bool RosaceShaderApp::Init()
{
	//m_bfullScreen = true;
	//m_width = 1920;
	//m_height = 1080;
	m_name = "RosaceShader";
	m_bfullScreen = false;
	m_width = 1280;
	m_height = 720;


	RecordableApp::Init();


	MapUniform uniformMap;

	uniformMap[SHADER_TIME]= UniformVar(eZGLtypeUniform::ZGL_FVEC1);
	uniformMap[SHADER_SCALE] = UniformVar(eZGLtypeUniform::ZGL_FVEC2);
	uniformMap[SHADER_NB_PT] = UniformVar(eZGLtypeUniform::ZGL_IVEC1);

	m_RosaceShader.Init("Rosace", false, uniformMap);
	
	glm::vec2 scale = glm::vec2(float(m_width) / float(m_height), 1.);
	if (m_width > m_height)
	{
		scale = glm::vec2(1.,float(m_height) / float(m_width));
	}

	m_RosaceShader.Enable();
	m_RosaceShader.updateUniform(SHADER_SCALE, &scale);
    m_RosaceShader.updateUniform(SHADER_NB_PT, &m_nbPt);

	{
		std::vector<glm::vec2> quad_vertex_buffer_data = {
			glm::vec2(-1.0f , -1.0f),
			glm::vec2(1.0f , -1.0f),
			glm::vec2(-1.0f ,  1.0f),
			glm::vec2(-1.0f ,  1.0f),
			glm::vec2(1.0f , -1.0f),
			glm::vec2(1.0f ,  1.0f)
		};
		
		
		
		
		// pos
		ZGLStride stride1;
		stride1.m_offset = sizeof(glm::vec2),
			stride1.m_type = GL_FLOAT;
		stride1.m_size = 2;
		
		
		ZGLVAODrawableParam paramDrawable;

		paramDrawable.m_stride = sizeof(glm::vec2);
		paramDrawable.m_nbVertex = quad_vertex_buffer_data.size();
		paramDrawable.m_pVertices = (void *)&quad_vertex_buffer_data[0];
		paramDrawable.m_strides = { stride1};

		m_quad.Init(paramDrawable);
	}

	m_FBO.Init(m_width, m_height);
	TexParam texparameter;
	texparameter.m_width = m_width;
	texparameter.m_height = m_height;
	texparameter.m_channel = GL_RED;
	texparameter.m_type = GL_FLOAT;
	m_tex.Init(texparameter);

	return true;
}

void RosaceShaderApp::OpenGLRender()
{
	setTargetRender();
	glClearColor(0.,0.,0.,1.);
	m_RosaceShader.Enable();
	float t = 0.5*m_cumulTime;
	m_RosaceShader.updateUniform(SHADER_TIME, &t);
	m_quad.Render(GL_TRIANGLES);


	
	

}

void RosaceShaderApp::ImguiDraw()
{
	RecordableApp::ImguiDraw();
	ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);



	if (ImGui::SliderInt("nb pt ", &m_nbPt, 100, 1000))
	{
		m_RosaceShader.updateUniform(SHADER_NB_PT, &m_nbPt);
	}

	


	ImGui::End();
}
