#pragma once
#include <ZGL/ZGLApp.h>
#include <ZGL/Shaders.h>

#include "ImageCapture.h"
#include "MyVideoCapture.h"

class RecordableApp : public ZGLApp
{
public:
	RecordableApp();
	~RecordableApp();

	virtual bool Init() override;
	virtual void PostProcess() override;

	virtual void KeyCallback(int key, int scancode, int action, int mods) override;

	virtual void ImguiDraw() override;

protected :

	//must be called by child class to allow recording 
	// screen or different FBO for recording
	void setTargetRender();


private :
	ImageCapture m_captureIm;
	MyVideoCapture m_captureVideo;

	bool m_bImageCapture = false;

	bool m_bRecord;
	bool m_bendRecord;

	Shader m_shaderQuad;
};

