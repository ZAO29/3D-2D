#pragma once
#include  <RecordableApp/RecordableApp.h>
#include <ZGL/ZGLDrawable.h>
#include <RecordableApp/LoadableTexture.h>

#include "SceneGraph.h"


struct CrossParam
{
	float m_scale = 0.2f;
	float m_specPow = 1.0f;
	float m_specIntensity = 0.0f;
	float m_tessCross = 1.0f;
	float m_tessCrossMax = 1000.f;
	float m_sizeCross = 0.0f;
	float m_reflectWeight = 0.5f;
};


struct CrossFieldParam
{
	float step = 0.0f;
	int nb = 11;
};

struct FogParam
{
	float altmax = 10.0f;
	float altmin = 0.0f;
	float density = 0.1f;
};

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

	void InitGround();
	void InitSkyBox();
	void InitCrossField();

	void RenderGround();
	void RenderSkybox();
	void RenderCrossField();

	void DestroyGround();

	Shader m_groundShader;
	ZGLVAODrawable* m_pground;

	ZGLIndexedVAODrawable* m_ppyramid;
	SceneGraph* m_psgraph;
	Shader m_shader;
	Shader m_shaderSkyBox;
	CrossParam m_crossParam;
	CrossFieldParam m_crossFieldParam;
	float m_tessSkybox = 50.0f;

	glm::vec3 m_dirLight = glm::vec3(1.f, 1.f, 1.f);

	FogParam m_fog;

};

