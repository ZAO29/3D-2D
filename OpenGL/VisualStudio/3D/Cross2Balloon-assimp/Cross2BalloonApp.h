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
    int nb = 3;
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
	void RenderGround();
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

};

