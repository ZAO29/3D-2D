#pragma once
#include  <RecordableApp/RecordableApp.h>
#include <ZGL/ZGLDrawable.h>
#include <RecordableApp/LoadableTexture.h>

class PyramidToSphereApp : public RecordableApp
{
public:
	PyramidToSphereApp();
	~PyramidToSphereApp();


	virtual bool Init() override;
	virtual void OpenGLRender() override;
	virtual void Destroy() override;
	virtual void ImguiDraw() override;

private :
	ZGLVAODrawable m_pyramid;
	Shader m_shader;
	float m_tessLevel = 1.0;
	float m_multColor = 0.5;

	LoadableTexture m_tex;
};

