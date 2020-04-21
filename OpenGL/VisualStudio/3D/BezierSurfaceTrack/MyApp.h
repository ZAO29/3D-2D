#pragma once
#include <ZGL/ZGLApp.h>
#include <ZGL/Shaders.h>
#include <ZGL/ZGLDrawable.h>
#include "BezierCurveDrawable.h"
#include "BezierSurfaceDrawable.h"
#include "PieceWiseBezierSurfaceDrawable.h"

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
	BezierCurveDrawable m_bezierCurve;
	BezierSurfaceDrawable m_bezierSurface;
	PieceWiseBezierSurfaceDrawable m_pwbezSurf;
	ctrl m_ctrl;
};

