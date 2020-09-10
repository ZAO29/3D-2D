#pragma once
#include  <RecordableApp/RecordableApp.h>
#include <ZGL/ZGLDrawable.h>
#include <RecordableApp/LoadableTexture.h>

#include "SceneGraph.h"

class Cross2BalloonApp : public RecordableApp
{
public:
	Cross2BalloonApp();
	~Cross2BalloonApp();

	virtual bool Init() override;
	virtual void OpenGLRender() override;
	virtual void Destroy() override;
	virtual void ImguiDraw() override;

private :
	ZGLIndexedVAODrawable* m_ppyramid;
	SceneGraph* m_psgraph;
	Shader m_shader;
	Shader m_shaderSkyBox;
	float m_scale = 0.2;
	float m_specPow = 1.0;
	float m_specIntensity = 0.0;
	float m_tessSkybox = 50.0;
	float m_tessCross= 1.0;
	float m_tessCrossMax = 1000.f;
	glm::vec3 m_dirLight = glm::vec3(1., 1., 1.);
	float m_sizeCross = 0.0;
	float m_reflectWeight = 0.5;
};

