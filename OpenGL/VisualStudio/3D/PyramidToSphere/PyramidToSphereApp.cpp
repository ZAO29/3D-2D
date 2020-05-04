#include "stdafx.h"
#include "PyramidToSphereApp.h"
#include <math.h>

#include <gtc/type_ptr.hpp>





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

	std::vector<glm::vec2> vert =
	{
		pos0,pos1,pos2,
		pos0,pos2,pos3,
		pos0,pos3,pos1,
		pos1,pos2,pos3

	};

	ZGLStride stride1;
	stride1.m_offset = sizeof(glm::vec2),
	stride1.m_type = GL_FLOAT;
	stride1.m_size = 2;

	ZGLVAODrawableParam paramDrawable;
	paramDrawable.m_stride = sizeof(glm::vec2);
	paramDrawable.m_nbVertex = vert.size();
	paramDrawable.m_pVertices = (void *)&vert[0];
	paramDrawable.m_strides = { stride1 };

	m_pyramid.Init(paramDrawable);

	MapUniform uniformMap;
	uniformMap[SHADER_MVP] = eZGLtypeUniform::ZGL_FMAT4;
	m_shader.Init("pyramid2sphere", uniformMap);

	return true;
}

void PyramidToSphereApp::OpenGLRender()
{
	RecordableApp::setTargetRender();
	m_shader.Enable();
	glm::mat4 mvp = m_pCam->getProjectionView() * m_pCam->getView();
	m_shader.updateUniform(SHADER_MVP, glm::value_ptr(mvp));
	glDisable(GL_CULL_FACE);
	m_pyramid.Render(GL_TRIANGLES);
}

void PyramidToSphereApp::Destroy()
{
}

void PyramidToSphereApp::ImguiDraw()
{
	RecordableApp::ImguiDraw();
}
