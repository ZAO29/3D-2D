#pragma once

#include <ZGL/ZGLApp.h>
#include <ZGL/ZGLDrawable.h>
#include <ZGL/Shaders.h>
#include <ZGL/Texture.h>
#include <ZGL/FBO.h>

class FractalCubeApp : public ZGLApp
{
public:
	FractalCubeApp();
	~FractalCubeApp();

	virtual bool Init() override;
	virtual void OpenGLRender() override;
	virtual void Destroy() override;
	virtual void ImguiDraw() override;

private :
	ZGLVAODrawable m_cube;
	ZGLVAODrawable m_quad;
	Shader m_shader;
	Shader m_motionBlurShader;
	Texture* m_ptex;
	FBO m_FBO[2];
	int m_nbOctave = 100;

	std::vector<FBO> m_MotionBlurFBOs;
	int m_nbMotionBlur = 1;
	int m_idFBO = 0;

};
