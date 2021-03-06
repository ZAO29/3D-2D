#include "stdafx.h"
#include "PyramidToSphereApp.h"
#include <math.h>

#include <gtc/type_ptr.hpp>
#include <ZGL/imgui/imgui.h>


#define SHADER_TESSLEVEL "utessLevel"
#define SHADER_SUBDIV "usubdiv"

struct VertexPyrData
{
	VertexPyrData() {}
	VertexPyrData(glm::vec2 HP, int colorID) :m_headingPitch(HP), m_colorID(colorID) {}

	glm::vec2 m_headingPitch;
	float m_colorID;
};



PyramidToSphereApp::PyramidToSphereApp()
{
}


PyramidToSphereApp::~PyramidToSphereApp()
{
}

bool PyramidToSphereApp::Init()
{
	m_name = "PyramidToSphereApp";
	m_bfullScreen = false;
	m_width = 1280;
	m_height = 720;

	//m_bfullScreen = true;
	//m_width = 1920;
	//m_height = 1080;

	m_typeCamera = eCameraType::TRACKBALLCAMERA;

	RecordableApp::Init();

	m_pCam->setEyePos(glm::vec3(5.0, 0., 0.));
	//m_method1.Init();
	
	m_method2.Init();

	m_Tex.Load("D:/Pictures/test/sky.jpg");
	//m_Tex.Load("D:/OUTPUT/PyramidToSphereApp_2.jpg");
	InitQuad(m_quad);
	m_quadShader.Init("quad");
	return true;
}

void PyramidToSphereApp::OpenGLRender()
{
	RecordableApp::setTargetRender();
	//m_method1.Render(m_pCam);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_quadShader.Enable();
	glDisable(GL_DEPTH_TEST);
	m_quad.Render(GL_TRIANGLES);
	m_Tex.Bind(GL_TEXTURE0);
	m_method2.Render(m_pCam);

}

void PyramidToSphereApp::Destroy()
{
	m_method2.Destroy();
	m_pWindowEnv->destroy();
}

void PyramidToSphereApp::ImguiDraw()
{
	RecordableApp::ImguiDraw();

	ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
	//m_method1.ImguiDraw();
	m_method2.ImguiDraw();
	ImGui::End();
	
}

void SubdivisionMethod1::Init()
{
	float PitchBase = PI / 2. - acos(-1. / 3.);

	glm::vec2 pos0(0, PI / 2.);
	glm::vec2 pos1(0, PitchBase);
	glm::vec2 pos2(2.*PI / 3., PitchBase);
	glm::vec2 pos3(4.*PI / 3., PitchBase);

	std::vector<VertexPyrData> vert =
	{
		VertexPyrData(pos0,0),VertexPyrData(pos1,0),VertexPyrData(pos2,0),
		VertexPyrData(pos0,1),VertexPyrData(pos2,1),VertexPyrData(pos3,1),
		VertexPyrData(pos0,2),VertexPyrData(pos3,2),VertexPyrData(pos1,2),
		VertexPyrData(pos1,3),VertexPyrData(pos2,3),VertexPyrData(pos3,3)

	};

	ZGLStride stride1, stride2;
	stride1.m_offset = sizeof(glm::vec2);
	stride1.m_type = GL_FLOAT;
	stride1.m_size = 2;

	stride2.m_offset = sizeof(float);
	stride2.m_type = GL_FLOAT;
	stride2.m_size = 1;

	ZGLVAODrawableParam paramDrawable;
	paramDrawable.m_stride = sizeof(VertexPyrData);
	paramDrawable.m_nbVertex = vert.size();
	paramDrawable.m_pVertices = (void *)&vert[0];
	paramDrawable.m_strides = { stride1,stride2 };

	m_pyramid.Init(paramDrawable);

	MapUniform uniformMap;
	uniformMap[SHADER_MVP] = eZGLtypeUniform::ZGL_FMAT4;
	uniformMap[SHADER_TESSLEVEL] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_SUBDIV] = eZGLtypeUniform::ZGL_FVEC1;
	GraphicPipelineType shaderType;
	shaderType.tesCtrl = true;
	shaderType.tesEval = true;
	shaderType.geometry = true;
	m_shader.Init("pyramid2sphere", uniformMap, shaderType);
	m_shader.Enable();
	m_shader.updateUniform(SHADER_TESSLEVEL, &m_tessLevel);
	m_shader.updateUniform(SHADER_SUBDIV, &m_subdiv);


	m_tex.Load("D://Textures//2k_mercury.jpg");
	//m_tex.Load("D://Textures//degrade.jpg");
}

void SubdivisionMethod1::Render(Camera* pCam)
{
	m_shader.Enable();
	glm::mat4 mvp = pCam->getProjectionAndView();
	m_shader.updateUniform(SHADER_MVP, glm::value_ptr(mvp));
	
	m_tex.Bind(GL_TEXTURE0);
	
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	m_pyramid.Render(GL_PATCHES);
}

void SubdivisionMethod1::ImguiDraw()
{
	if (ImGui::SliderFloat("tesselation level", &m_tessLevel, 1.0, 100.0))
	{
		m_shader.Enable();
		m_shader.updateUniform(SHADER_TESSLEVEL, &m_tessLevel);
	}

	if (ImGui::SliderFloat("subdiv (manual tessellation) ", &m_subdiv, 0.0, 3.))
	{
		m_shader.Enable();
		m_shader.updateUniform(SHADER_SUBDIV, &m_subdiv);
	}

	
}
