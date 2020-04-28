#pragma once
#include <ZGL/ZGLApp.h>
#include <ZGL/Shaders.h>
#include <ZGL/ZGLDrawable.h>
#include <ZGL/FBO.h>
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

	void UpdateMultiUse(float elapsedTime);

	void InitCube();
	Shader m_shader[2];
	ZGLVAODrawable m_VAOdrawable;

	
	
	PieceWiseBezierCurveDrawable<glm::vec2> m_trackDrawable;
	PieceWiseBezierSurfaceDrawable m_pwbezSurf;
	ctrl m_ctrl;
	FBO m_FBO;
	Shader m_quadShader;
	ZGLVAODrawable m_quad;
	glm::vec4 m_offsetScale = glm::vec4(-1, -1, 0.5, 0.5);
	glm::vec4 m_offsetScaleCurve = glm::vec4(0, 0, 1, 1);
	float m_visi = 0.1;
	glm::vec4 m_multiUse = glm::vec4(1.,0.,0.,1.);
	bool m_idShader = false;



};

