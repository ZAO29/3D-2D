#pragma once
#include  <RecordableApp/RecordableApp.h>
#include <ZGL/ZGLDrawable.h>
#include <RecordableApp/LoadableTexture.h>


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
	ZGLIndexedVAODrawable m_pyramid;
	Shader m_shader;
};

