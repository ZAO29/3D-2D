#pragma once
#include "ZGLApp.h"
#include "Shaders.h"
#include "ZGLDrawable.h"

struct ctrl
{
	int m_selectedCam;
};



class MyApp :
	public ZGLApp
{
public:
	MyApp();
	~MyApp();

	virtual bool Init() override;
	virtual void OpenGLRender() override;
	virtual void Destroy() override;
	virtual void ImguiDraw() override;



protected :
	Shader m_shader;
	ZGLVAODrawable m_VAOdrawable;

	std::map<eCameraType, Camera*> m_CameraMap;
	Camera* m_pCam;

	ctrl m_ctrl;
};

