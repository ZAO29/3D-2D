#pragma once

#include <RecordableApp/RecordableApp.h>
#include <ZGL/Shaders.h>
#include <ZGL/ZGLDrawable.h>
#include <ZGL/FBO.h>
#include <ZGL/Texture.h>
#include <RecordableApp/ImageCapture.h>

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
	Shader m_compoShader;
	
	ImageCapture m_capture;

	Texture m_texR;
	Texture m_texG;
	Texture m_texB;
	int m_nbPt = 300;
	int m_kernelSize = 20;
	glm::ivec2 m_kernelMult = glm::ivec2(2, 3);
	float m_reinitTime = 0;
	float m_speed = 0.5;
	bool m_equalize = true;
};

