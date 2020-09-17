#include "stdafx.h"
#include "Cross2BalloonApp.h"

#include <gtc/type_ptr.hpp>

#include <ZGL/imgui/imgui.h>
#include <gtx/transform.hpp>

#define SHADER_MODEL "uModel"
#define SHADER_CAMPOS "uCamPos"
#define SHADER_SPECPOW "uSpecularPow"
#define SHADER_SPECINTENSITY "uSpecularIntensity"
#define SHADER_DIRLIGHT "uLightDir"
#define SHADER_CENTERRADIUS "uCenterRadius"
#define SHADER_REFLECTIONWEIGHT "uReflectWeight"

#define SHADER_TESS "utessLevel"
#define SHADER_TESSMAX "utessLevelMax"


#define SHADER_FOG_ALTMIN "uFogAltMin"
#define SHADER_FOG_ALTMAX "uFogAltMax"
#define SHADER_FOG_DENSITY "uFogDensity"

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
	
	//m_bfullScreen = true;
	//m_width = 1920;
	//m_height = 1080;
	m_typeCamera = eCameraType::FREECAMERA;
	m_bfixedTime = false;


	RecordableApp::Init();


	m_pCam->setEyePos(glm::vec3(-5.0, 10., -5.));
	m_pCam->setDirection(glm::vec3(1.0, -2., 1.));
	m_pCam->m_speed = 15.;

	SceneGraph::sInitBoundingBoxCube();

	UniformVarValueHolder::sAdd(eZGLtypeUniform::ZGL_FVEC1, SHADER_FOG_ALTMAX);
	UniformVarValueHolder::sAdd(eZGLtypeUniform::ZGL_FVEC1, SHADER_FOG_ALTMIN);
	UniformVarValueHolder::sAdd(eZGLtypeUniform::ZGL_FVEC1, SHADER_FOG_DENSITY);
	UniformVarValueHolder::sAdd(eZGLtypeUniform::ZGL_FVEC3, SHADER_CAMPOS);

	UniformVarValueHolder::sUpdate(SHADER_FOG_ALTMAX, &m_fog.altmax);
	UniformVarValueHolder::sUpdate(SHADER_FOG_ALTMIN, &m_fog.altmin);
	UniformVarValueHolder::sUpdate(SHADER_FOG_DENSITY, &m_fog.density);
	glm::vec3 posCam = m_pCam->getEyePos();
	UniformVarValueHolder::sUpdate(SHADER_CAMPOS, &posCam);


	InitCrossField();

	InitSkyBox();

	InitGround();

	return true;
}

void Cross2BalloonApp::OpenGLRender()
{
	RecordableApp::setTargetRender();

	glm::vec3 eyePos = m_pCam->getEyePos();
	UniformVarValueHolder::sUpdate(SHADER_CAMPOS, &eyePos);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	RenderSkybox();

	glEnable(GL_DEPTH_TEST);
	
	RenderCrossField();

	RenderGround();

	


}

void Cross2BalloonApp::Destroy()
{
	m_ppyramid->Destroy();
	m_psgraph-> Destroy();
	DestroyGround();
	m_pWindowEnv->destroy();
}

void Cross2BalloonApp::ImguiDraw()
{
	RecordableApp::ImguiDraw();
	ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::TreeNode("Cross"))
	{
		ImGui::SliderFloat("scale", &m_crossParam.m_scale, 0.1f, 10.f);
		ImGui::SliderFloat("specular power", &m_crossParam.m_specPow, 0.1f, 100.f);
		ImGui::SliderFloat("specular intensity", &m_crossParam.m_specIntensity, 0.0f, 1.f);
		ImGui::SliderFloat("tesselation cross", &m_crossParam.m_tessCross, 1.f, m_crossParam.m_tessCrossMax);
		ImGui::SliderFloat("tesselation cross max", &m_crossParam.m_tessCrossMax, 1.f, 1000.f);
		ImGui::SliderFloat(" reflection weight", &m_crossParam.m_reflectWeight, 0., 1.);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("CrossField"))
	{
		ImGui::SliderFloat("step", &m_crossFieldParam.step, m_crossParam.m_sizeCross, 10.f*m_crossParam.m_sizeCross);
		ImGui::SliderInt("number", &m_crossFieldParam.nb, 3,100);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Fog"))
	{
		if (ImGui::SliderFloat("minimum altitude ", &m_fog.altmin, 0.0f, m_fog.altmax))
		{
			UniformVarValueHolder::sUpdate(SHADER_FOG_ALTMIN, &m_fog.altmin);
		}
		if (ImGui::SliderFloat("maximum altitude", &m_fog.altmax, m_fog.altmin, 5.f))
		{
			UniformVarValueHolder::sUpdate(SHADER_FOG_ALTMAX, &m_fog.altmax);
		}
		if (ImGui::SliderFloat("density", &m_fog.density, 0.01f, 1.f))
		{
			UniformVarValueHolder::sUpdate(SHADER_FOG_DENSITY, &m_fog.density);
		}
		ImGui::TreePop();
	}

	if (ImGui::SliderFloat3("dir light", &m_dirLight[0], -1.f, 1.f))
	{
		m_dirLight=glm::normalize(m_dirLight);
	}
	m_psgraph->ImguiDraw();
	ImGui::End();
	

}

void Cross2BalloonApp::InitGround()
{
	ZGLVAODrawableParam paramDrawable;

	std::vector<glm::vec3> vertices =
	{
		glm::vec3(-0.5,0.,0.5),
		glm::vec3(-0.5,0.,-0.5),
		glm::vec3(0.5,0.,0.5),
		glm::vec3(0.5,0.,-0.5)
	};

	ZGLStride stride1;
	stride1.m_offset = sizeof(glm::vec3);
	stride1.m_type = GL_FLOAT;
	stride1.m_size = 3;

	paramDrawable.m_stride = sizeof(glm::vec3);
	paramDrawable.m_nbVertex = static_cast<unsigned int>(vertices.size());
	paramDrawable.m_pVertices = (void *)&vertices[0];
	paramDrawable.m_strides = { stride1 };


	m_pground = new ZGLVAODrawable();
	m_pground->Init(paramDrawable);


	MapUniform uniformMap;
	uniformMap[SHADER_MVP] = eZGLtypeUniform::ZGL_FMAT4;
	uniformMap[SHADER_MODEL] = eZGLtypeUniform::ZGL_FMAT4;
	


	GraphicPipelineType shaderType;

	std::vector<std::string> subscribedUniformNames = { SHADER_FOG_ALTMAX, 
														SHADER_FOG_ALTMIN, 
														SHADER_FOG_DENSITY, 
														SHADER_CAMPOS};

	m_groundShader.Init("Ground", uniformMap, shaderType, subscribedUniformNames);
}

void Cross2BalloonApp::InitSkyBox()
{
	ZGLStride stride1;
	stride1.m_offset = sizeof(glm::vec2);
	stride1.m_type = GL_FLOAT;
	stride1.m_size = 2;

	float scale = 1.0f;

	float PitchBase = PI / 2.f - acos(-1.f / 3.f);


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
	paramDrawable.m_param.m_nbVertex = static_cast<unsigned int>(vertices.size());
	paramDrawable.m_param.m_pVertices = (void *)&vertices[0];
	paramDrawable.m_param.m_strides = { stride1 };
	paramDrawable.m_nbIndices = static_cast<unsigned int>(indices.size());
	paramDrawable.m_pIndices = &indices[0];

	m_ppyramid = new ZGLIndexedVAODrawable();
	m_ppyramid->Init(paramDrawable);




	// shader
	MapUniform uniformMapSkybox;
	GraphicPipelineType shaderTypeSkybox;
	shaderTypeSkybox.tesCtrl = true;
	shaderTypeSkybox.tesEval = true;

	uniformMapSkybox[SHADER_MVP] = eZGLtypeUniform::ZGL_FMAT4;
	uniformMapSkybox[SHADER_TESS] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMapSkybox[SHADER_DIRLIGHT] = eZGLtypeUniform::ZGL_FVEC3;
	m_shaderSkyBox.Init("Skybox", uniformMapSkybox, shaderTypeSkybox);

}

void Cross2BalloonApp::InitCrossField()
{
	MapUniform uniformMap;
	uniformMap[SHADER_MVP] = eZGLtypeUniform::ZGL_FMAT4;
	uniformMap[SHADER_MODEL] = eZGLtypeUniform::ZGL_FMAT4;
	uniformMap[SHADER_DIRLIGHT] = eZGLtypeUniform::ZGL_FVEC3;
	uniformMap[SHADER_SPECPOW] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_SPECINTENSITY] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_TESS] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_TESSMAX] = eZGLtypeUniform::ZGL_FVEC1;
	uniformMap[SHADER_CENTERRADIUS] = eZGLtypeUniform::ZGL_FVEC4;
	uniformMap[SHADER_REFLECTIONWEIGHT] = eZGLtypeUniform::ZGL_FVEC1;



	GraphicPipelineType shaderType;
	shaderType.tesCtrl = true;
	shaderType.tesEval = true;

	std::vector<std::string> subscribedUniformNames = { SHADER_FOG_ALTMAX,
														SHADER_FOG_ALTMIN, 
														SHADER_FOG_DENSITY,
														SHADER_CAMPOS};

	m_shader.Init("shader", uniformMap, shaderType, subscribedUniformNames);
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



	m_crossParam.m_sizeCross = std::max(s.x, std::max(s.y, s.z));


	m_crossFieldParam.step = 5 * m_crossParam.m_sizeCross;
	glm::vec4 centerRadius = glm::vec4(center.x, center.y, center.z, m_crossParam.m_sizeCross);

	// leger shift pour que la croix soit etoilee autour de son centre
	centerRadius.y += s.y / 8.0f;
	m_shader.updateUniform(SHADER_CENTERRADIUS, &centerRadius);

}

void Cross2BalloonApp::RenderGround()
{
	m_groundShader.Enable();
	
	glm::mat4 model =  glm::translate(glm::vec3(0.5f, 0.f, 0.5f));
	model = glm::scale(m_crossParam.m_scale*((m_crossFieldParam.nb - 1.f)*m_crossFieldParam.step)*glm::vec3(1.f)) * model;
	
	glm::mat4 mvp = m_pCam->getProjectionAndView()*model;

	m_groundShader.updateUniform(SHADER_MVP, glm::value_ptr(mvp));
	m_groundShader.updateUniform(SHADER_MODEL, glm::value_ptr(model));


	m_pground->Render(GL_TRIANGLE_STRIP);
}

void Cross2BalloonApp::RenderSkybox()
{
	m_shaderSkyBox.Enable();
	glm::mat4 mvp = m_pCam->getProjectionAndView()*glm::translate(m_pCam->getEyePos());
	m_shaderSkyBox.updateUniform(SHADER_MVP, glm::value_ptr(mvp));
	m_shaderSkyBox.updateUniform(SHADER_TESS, &m_tessSkybox);
	m_shaderSkyBox.updateUniform(SHADER_DIRLIGHT, &m_dirLight);


	glPatchParameteri(GL_PATCH_VERTICES, 3);
	m_ppyramid->Render(GL_PATCHES);
}

void Cross2BalloonApp::RenderCrossField()
{
	m_shader.Enable();
	glm::mat4 scale = glm::scale(m_crossParam.m_scale*glm::vec3(1.));

	glm::mat4 model = scale;
	glm::mat4 vp = m_pCam->getProjectionAndView();
	glm::mat4 mvp = vp * model;


	m_shader.updateUniform(SHADER_MODEL, glm::value_ptr(model));
	m_shader.updateUniform(SHADER_MVP, glm::value_ptr(mvp));
	m_shader.updateUniform(SHADER_SPECPOW, &m_crossParam.m_specPow);
	m_shader.updateUniform(SHADER_SPECINTENSITY, &m_crossParam.m_specIntensity);
	m_shader.updateUniform(SHADER_DIRLIGHT, &m_dirLight);
	m_shader.updateUniform(SHADER_TESS, &m_crossParam.m_tessCross);
	m_shader.updateUniform(SHADER_TESSMAX, &m_crossParam.m_tessCrossMax);
	m_shader.updateUniform(SHADER_REFLECTIONWEIGHT, &m_crossParam.m_reflectWeight);

	
	glm::mat4  trans_x = glm::translate(glm::vec3(m_crossFieldParam.step, 0, 0));
	glm::mat4  trans_y = glm::translate(glm::vec3(0, 0, m_crossFieldParam.step));
	glm::mat4 model_x = glm::mat4(1.);
	glm::mat4 mvp_final;
	glm::mat4 model_final;

	float t = m_cumulTime;

	glm::vec2 pos_grid(0.);

	glm::vec2 center = (m_crossFieldParam.step*((float(m_crossFieldParam.nb) - 1.f) / 2.f)) * glm::vec2(1.);

	//LOOP : TODO should use innstancing instead
	for (int j = 0; j < m_crossFieldParam.nb; j++)
	{

		model_x = model;
		pos_grid.x = 0.;

		for (int i = 0; i < m_crossFieldParam.nb; i++)
		{
			float dist = glm::distance(center, pos_grid);
			float y = std::max(3.f*cos(dist / 100.f + t * 0.8f), 0.f) / (0.1f + dist / 100.f);
			model_final = model_x * glm::translate(glm::vec3(0.f, 5.f*m_crossParam.m_sizeCross * std::sqrt(y), 0.));
			mvp_final = vp * model_final;
			m_crossParam.m_tessCross = y / 3.0f*(float(m_crossParam.m_tessCrossMax) - 1.f) + 1.f;
			m_crossParam.m_tessCross = std::min(m_crossParam.m_tessCross, m_crossParam.m_tessCrossMax);
			m_shader.updateUniform(SHADER_TESS, &m_crossParam.m_tessCross);
			m_shader.updateUniform(SHADER_MVP, glm::value_ptr(mvp_final));
			m_shader.updateUniform(SHADER_MODEL, glm::value_ptr(model_final));

			m_psgraph->Render(GL_PATCHES);

			model_x = model_x * trans_x;
			pos_grid += glm::vec2(m_crossFieldParam.step, 0.);

		}
		model = model * trans_y;
		pos_grid += glm::vec2(0., m_crossFieldParam.step);

	}
}

void Cross2BalloonApp::DestroyGround()
{
	if (m_pground != nullptr)
	{
		m_pground->Destroy();
		m_pground = nullptr;
	}
}
