#include "stdafx.h"
#include "Cross2BalloonApp.h"

#include <gtc/type_ptr.hpp>

#include <ZGL/imgui/imgui.h>
#include <gtx/transform.hpp>
#define SHADER_MV "uModelView"
#define SHADER_MODEL "uModel"
#define SHADER_CAMPOS "uCamPos"
#define SHADER_SPECPOW "uSpecularPow"
#define SHADER_SPECINTENSITY "uSpecularIntensity"
#define SHADER_DIRLIGHT "uLightDir"
#define SHADER_CENTERRADIUS "uCenterRadius"

#define SHADER_TESS "utessLevel"
#define SHADER_TESSMAX "utessLevelMax"

Cross2BalloonApp::Cross2BalloonApp()
{
}


Cross2BalloonApp::~Cross2BalloonApp()
{
}


struct VerticeData
{
	VerticeData(glm::vec2 hp) :m_headingPitch(hp) {}

	glm::vec2 m_headingPitch;
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
	stride1.m_offset = sizeof(glm::vec2);
	stride1.m_type = GL_FLOAT;
	stride1.m_size = 2;

	float scale = 1.0f;

	float PitchBase = PI / 2. - acos(-1. / 3.);


	glm::vec2 pos0(0, PI / 2.);
	glm::vec2 pos1(0, PitchBase);
	glm::vec2 pos2(2.*PI / 3., PitchBase);
	glm::vec2 pos3(4.*PI / 3., PitchBase);

				

	std::vector<VerticeData> vertices =
	{ VerticeData(pos1),VerticeData(pos2),VerticeData(pos3),VerticeData(pos0)
	};

	std::vector<unsigned int> indices = { 0,1,2,
						   0,1,3,
						   1,2,3,
						   2,0,3};

	ZGLVAOIndexedDrawableParam paramDrawable;

	paramDrawable.m_param.m_stride = sizeof(VerticeData);
	paramDrawable.m_param.m_nbVertex = vertices.size();
	paramDrawable.m_param.m_pVertices = (void *)&vertices[0];
	paramDrawable.m_param.m_strides = { stride1};
	paramDrawable.m_nbIndices = indices.size();
	paramDrawable.m_pIndices = &indices[0];

	m_ppyramid = new ZGLIndexedVAODrawable();
	m_ppyramid->Init(paramDrawable);

	MapUniform uniformMap;
	uniformMap[SHADER_MVP] = eZGLtypeUniform::ZGL_FMAT4;
	uniformMap[SHADER_MODEL] = eZGLtypeUniform::ZGL_FMAT4;
	uniformMap[SHADER_CAMPOS] = eZGLtypeUniform::ZGL_FVEC3;
	uniformMap[SHADER_DIRLIGHT] = eZGLtypeUniform::ZGL_FVEC3;
	uniformMap[SHADER_SPECPOW] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_SPECINTENSITY] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_TESS] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_TESSMAX] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_CENTERRADIUS] = eZGLtypeUniform::ZGL_FVEC4;
	GraphicPipelineType shaderType;
	shaderType.tesCtrl = true;
	shaderType.tesEval = true;
	m_shader.Init("shader", uniformMap, shaderType);
	m_shader.Enable();
	

	//boundingBox
	m_psgraph = new SceneGraph();
	m_psgraph->loadModel("D:/Repos/2D-3D/blender/blender_script/UnikCross.fbx");
	auto bb = m_psgraph->m_bbox;
	std::cout << " **** Bounding Box center " << bb.getCenter<float>().x << "|"
		<< bb.getCenter<float>().y << "|"
		<< bb.getCenter<float>().z << "|" << std::endl;
	glm::vec3 s = bb.getSize();
	std::cout << " Bounding Box size **** " << s.x << "|"
		<< s.y << "|"
		<< s.z << "|" << std::endl;
	glm::vec3 center = bb.getCenter<float>();


	float size = std::max(s.x, std::max(s.y, s.z));
	glm::vec4 centerRadius = glm::vec4(center.x, center.y, center.z, size);
	
	// leger shift pour que la croix soit etoilee autour de son centre
	centerRadius.y += s.y / 8.0;
	m_shader.updateUniform(SHADER_CENTERRADIUS, &centerRadius);



	MapUniform uniformMapSkybox;
	GraphicPipelineType shaderTypeSkybox;
	shaderTypeSkybox.tesCtrl = true;
	shaderTypeSkybox.tesEval = true;
	
	uniformMapSkybox[SHADER_MVP] = eZGLtypeUniform::ZGL_FMAT4;
	uniformMapSkybox[SHADER_TESS] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMapSkybox[SHADER_DIRLIGHT] = eZGLtypeUniform::ZGL_FVEC3;
	m_shaderSkyBox.Init("Skybox", uniformMapSkybox,shaderTypeSkybox);

	SceneGraph::sInitBoundingBoxCube();
	

	return true;
}

void Cross2BalloonApp::OpenGLRender()
{
	RecordableApp::setTargetRender();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	
	{
		m_shaderSkyBox.Enable();
		glm::mat4 mvp = m_pCam->getProjectionView() * m_pCam->getView()*glm::translate(m_pCam->getEyePos());
		m_shaderSkyBox.updateUniform(SHADER_MVP, glm::value_ptr(mvp));
		m_shaderSkyBox.updateUniform(SHADER_TESS, &m_tessSkybox);
		m_shaderSkyBox.updateUniform(SHADER_DIRLIGHT, &m_dirLight);

		glPatchParameteri(GL_PATCH_VERTICES, 3);
		m_ppyramid->Render(GL_PATCHES);
	}




	glEnable(GL_DEPTH_TEST);
	m_shader.Enable();
	glm::mat4 scale = glm::scale(glm::vec3(m_scale, m_scale, m_scale));

	glm::mat4 mvp = m_pCam->getProjectionView() * m_pCam->getView() * scale;

	glm::vec3 eyepos = m_pCam->getEyePos();

	m_shader.updateUniform(SHADER_MODEL, glm::value_ptr(scale));
	m_shader.updateUniform(SHADER_MVP, glm::value_ptr(mvp));
	m_shader.updateUniform(SHADER_CAMPOS, &eyepos);
	m_shader.updateUniform(SHADER_SPECPOW, &m_specPow);
	m_shader.updateUniform(SHADER_SPECINTENSITY, &m_specIntensity);
	m_shader.updateUniform(SHADER_DIRLIGHT, &m_dirLight);
	m_shader.updateUniform(SHADER_TESS,&m_tessCross);
	m_shader.updateUniform(SHADER_TESSMAX, &m_tessCrossMax);

	glPatchParameteri(GL_PATCH_VERTICES, 3);
	m_psgraph->Render(GL_PATCHES);

	// bounding box
	m_psgraph->RenderBoundingBox(mvp);
}

void Cross2BalloonApp::Destroy()
{
	m_ppyramid->Destroy();
	m_psgraph-> Destroy();
	m_pWindowEnv->destroy();
}

void Cross2BalloonApp::ImguiDraw()
{
	RecordableApp::ImguiDraw();
	ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat("scale", &m_scale, 0.1f, 10.f);
	ImGui::SliderFloat("specular power", &m_specPow, 0.1f, 10.f);
	ImGui::SliderFloat("specular intensity", &m_specIntensity, 0.0f, 1.f);
	ImGui::SliderFloat("tesselation skybox", &m_tessSkybox, 0.0f, 10.f);
	ImGui::SliderFloat("tesselation cross", &m_tessCross, 1.f, m_tessCrossMax);
	ImGui::SliderFloat("tesselation cross max", &m_tessCrossMax, 1.f, 1000.f);
	if (ImGui::SliderFloat3("dir light", &m_dirLight[0], -1.f, 1.f))
	{
		m_dirLight=glm::normalize(m_dirLight);
	}
	m_psgraph->ImguiDraw();
	ImGui::End();
	

}
