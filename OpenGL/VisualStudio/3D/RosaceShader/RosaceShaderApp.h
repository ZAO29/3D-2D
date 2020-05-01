#pragma once

#include <RecordableApp/RecordableApp.h>
#include <ZGL/Shaders.h>
#include <ZGL/ZGLDrawable.h>
#include <ZGL/FBO.h>
#include <ZGL/Texture.h>

class RosaceShaderApp : public RecordableApp
{
public:
	RosaceShaderApp();
	~RosaceShaderApp();


	virtual bool Init() override;
	virtual void OpenGLRender() override;
	virtual void ImguiDraw() override;


	ZGLVAODrawable m_quad;
	Shader m_RosaceShader;
	FBO m_FBO;
	Texture m_tex;
	int m_nbPt = 500;
};

