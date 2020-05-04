#include "stdafx.h"
#include "PyramidToSphereApp.h"
#include <math.h>

#include <gtc/type_ptr.hpp>
#include <ZGL/imgui/imgui.h>


#define SHADER_TESSLEVEL "utessLevel"

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
	m_typeCamera = eCameraType::TRACKBALLCAMERA;

	RecordableApp::Init();

	m_pCam->setEyePos(glm::vec3(5.0, 0., 0.));

	float PitchBase = PI/2. - acos(-1. / 3.);

	glm::vec2 pos0(0, PI/2.);
	glm::vec2 pos1(0, PitchBase);
	glm::vec2 pos2(2.*PI/3., PitchBase);
	glm::vec2 pos3(4.*PI/3., PitchBase);

	std::vector<VertexPyrData> vert =
	{
		VertexPyrData(pos0,0),VertexPyrData(pos1,0),VertexPyrData(pos2,0),
		VertexPyrData(pos0,1),VertexPyrData(pos2,1),VertexPyrData(pos3,1),
		VertexPyrData(pos0,2),VertexPyrData(pos3,2),VertexPyrData(pos1,2),
		VertexPyrData(pos1,3),VertexPyrData(pos2,3),VertexPyrData(pos3,3)

	};

	ZGLStride stride1,stride2;
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
	GraphicPipelineType shaderType;
	shaderType.tesCtrl = true;
	shaderType.tesEval = true;
	m_shader.Init("pyramid2sphere", uniformMap, shaderType);
	m_shader.Enable();
	m_shader.updateUniform(SHADER_TESSLEVEL, &m_tessLevel);

	return true;
}

void PyramidToSphereApp::OpenGLRender()
{
	RecordableApp::setTargetRender();
	m_shader.Enable();
	glm::mat4 mvp = m_pCam->getProjectionView() * m_pCam->getView();
	m_shader.updateUniform(SHADER_MVP, glm::value_ptr(mvp));
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	m_pyramid.Render(GL_PATCHES);
}

void PyramidToSphereApp::Destroy()
{
}

void PyramidToSphereApp::ImguiDraw()
{
	RecordableApp::ImguiDraw();

	ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::SliderFloat("tesselation level", &m_tessLevel, 1.0, 100.0))
	{
		m_shader.Enable();
		m_shader.updateUniform(SHADER_TESSLEVEL, &m_tessLevel);
	}
	ImGui::End();
}
