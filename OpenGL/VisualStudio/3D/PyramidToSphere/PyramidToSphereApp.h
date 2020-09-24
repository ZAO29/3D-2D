#pragma once
#include  <RecordableApp/RecordableApp.h>
#include <ZGL/ZGLDrawable.h>
#include <RecordableApp/LoadableTexture.h>

#include "SubdivisionMethod2.h"

// METHOD 1 : 
//- make subdivision in the geometry shader (control by m_subdiv)
// inconvenient : in the geometry shader, if you reach the level 3,
// you reach HARDWARE LIMITATION, can only emit n vertices of this size
// or using tesselation shader : 
// inconvenient : you cannot set a new color for each level of subdivision
struct SubdivisionMethod1
{
	ZGLVAODrawable m_pyramid;
	Shader m_shader;
	float m_tessLevel = 1.0;
	float m_subdiv = 0.0;
	LoadableTexture m_tex;

	void Init();
	void Render(Camera* pCam);
	void ImguiDraw();
};


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
	
	SubdivisionMethod1 m_method1;
	SubdivisionMethod2 m_method2;
	
};

