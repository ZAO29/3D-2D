#pragma once

#include <ZGL/ZGLApp.h>
#include <ZGL/ZGLDrawable.h>
#include <ZGL/Shaders.h>
#include <ZGL/Texture.h>

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
	Texture* m_ptex;

};

