#pragma once

#include <RecordableApp/RecordableApp.h>
#include <ZGL/Shaders.h>
#include <ZGL/ZGLDrawable.h>
#include <ZGL/FBO.h>
#include <ZGL/Texture.h>
#include <RecordableApp/ImageCapture.h>


enum eBLURRING
{
	CPU,
	GPU
};

class RosaceShaderApp : public RecordableApp
{
public:
	RosaceShaderApp();
	~RosaceShaderApp();


	virtual bool Init() override;
	virtual void OpenGLRender() override;
	virtual void ImguiDraw() override;

	void CPUBlurr();

	void GPUBlurr();


	ZGLVAODrawable m_quad;
	Shader m_RosaceShader;
	Shader m_compoShader;
	
	ImageCapture m_capture;

	int m_nbPt = 300;


	Texture m_texRGB[3];
	

	eBLURRING m_blurTexnik = eBLURRING::GPU;

	// CPU GAUSSIAN BLURR
	int m_kernelSize = 20;
	glm::ivec2 m_kernelMult = glm::ivec2(2, 3);
	float m_reinitTime = 0;
	float m_speed = 0.5;
	bool m_equalize = true;
	int m_timeOffset = 0;


	// GPU GAUSSIAN BLURR
	Shader m_blurShader;
	FBO m_pingpongFBO[2];
	glm::ivec3 m_nbBlurFilter = glm::ivec3(10);
	glm::vec2 m_powMult = glm::vec2(1.);
	ImageCapture m_RGBFBO[3];


};

