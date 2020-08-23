#include "stdafx.h"
#include "Cross2BalloonApp.h"

#include <gtc/type_ptr.hpp>

Cross2BalloonApp::Cross2BalloonApp()
{
}


Cross2BalloonApp::~Cross2BalloonApp()
{
}


struct VerticeData
{
	VerticeData(glm::vec3 pos) :m_pos(pos) {}

	glm::vec3 m_pos;
};

bool Cross2BalloonApp::Init()
{
	m_name = "Cross2Balloon";
	m_bfullScreen = false;
	m_width = 1280;
	m_height = 720;
	m_typeCamera = eCameraType::TRACKBALLCAMERA;

	RecordableApp::Init();


	m_pCam->setEyePos(glm::vec3(5.0, 0., 0.));

	ZGLStride stride1;
	stride1.m_offset = sizeof(glm::vec3);
	stride1.m_type = GL_FLOAT;
	stride1.m_size = 3;

	float scale = 1.0f;
	std::vector<VerticeData> vertices = { glm::vec3(-scale,-scale,0),
								 glm::vec3(scale,-scale,0),
								 glm::vec3(scale,scale,0),
								 glm::vec3(-scale,scale,0),
								 glm::vec3(0,0,scale) };
							   

	std::vector<unsigned int> indices = { 0,1,2,
						   0,2,3,
						   0,1,4,
						   1,2,4,
						   2,3,4,
						   3,0,4 };

	ZGLVAOIndexedDrawableParam paramDrawable;

	paramDrawable.m_param.m_stride = sizeof(VerticeData);
	paramDrawable.m_param.m_nbVertex = vertices.size();
	paramDrawable.m_param.m_pVertices = (void *)&vertices[0];
	paramDrawable.m_param.m_strides = { stride1};
	paramDrawable.m_nbIndices = indices.size();
	paramDrawable.m_pIndices = &indices[0];

	m_pyramid.Init(paramDrawable);

	MapUniform uniformMap;
	uniformMap[SHADER_MVP] = eZGLtypeUniform::ZGL_FMAT4;
	GraphicPipelineType shaderType;
	/*shaderType.tesCtrl = true;
	shaderType.tesEval = true;
	shaderType.geometry = true;*/
	m_shader.Init("shader", uniformMap, shaderType);
	m_shader.Enable();
	
	return true;
}

void Cross2BalloonApp::OpenGLRender()
{
	RecordableApp::setTargetRender();
	m_shader.Enable();
	glm::mat4 mvp = m_pCam->getProjectionView() * m_pCam->getView();
	m_shader.updateUniform(SHADER_MVP, glm::value_ptr(mvp));

	//m_tex.Bind(GL_TEXTURE0);

	//glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	m_pyramid.Render(GL_TRIANGLES);
}

void Cross2BalloonApp::Destroy()
{
}

void Cross2BalloonApp::ImguiDraw()
{
}
