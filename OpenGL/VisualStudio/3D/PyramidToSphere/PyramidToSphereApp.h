#pragma once
#include  <RecordableApp/RecordableApp.h>
#include <ZGL/ZGLDrawable.h>

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
};

