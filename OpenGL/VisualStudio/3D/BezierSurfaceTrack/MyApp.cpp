#include "stdafx.h"

#include <algorithm>

#include "MyApp.h"

#include <ZGL/CameraFree.h>
#include <ZGL/CameraTrackBall.h>
#include "CameraPieceWiseBezierSurface.h"

#include <ZGL/imgui/imgui.h>

#include <gtc/type_ptr.hpp>

#include "Constant.h"

#include "PWBezierCurveParser.h"


#define SHADER_SIZE "uTime"
#define SHADER_MVP "uMVP"
#define SHADER_VISI "uVisi"
#define SHADER_OFFSET_SCALE "uOffsetScale"
#define SHADER_USETEX "uUseTex"







MyApp::MyApp()
{
}


MyApp::~MyApp()
{
}


bool MyApp::Init()
{
	ZGLApp::Init();
	// BLENDING
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//DEPTH
	glEnable(GL_DEPTH_TEST);

	MapUniform mapUniform;

	mapUniform[SHADER_SIZE] = UniformVar(eZGLtypeUniform::ZGL_FVEC1);
	mapUniform[SHADER_MVP] = UniformVar(eZGLtypeUniform::ZGL_FMAT4);
	mapUniform[SHADER_VISI] = UniformVar(eZGLtypeUniform::ZGL_FVEC1);

	m_shader[0].Init("shader", true, mapUniform);
	m_shader[0].Enable();
	m_shader[0].updateUniform(SHADER_VISI, (void *)&m_visi);
	MapUniform mapUniform1;

	mapUniform1[SHADER_SIZE] = UniformVar(eZGLtypeUniform::ZGL_FVEC1);
	mapUniform1[SHADER_MVP] = UniformVar(eZGLtypeUniform::ZGL_FMAT4);
	mapUniform1[SHADER_VISI] = UniformVar(eZGLtypeUniform::ZGL_FVEC1);
	m_shader[1].Init("shape", false, mapUniform1);
	m_shader[1].Enable();
	m_shader[1].updateUniform(SHADER_VISI, (void *)&m_visi);
	float a = 1.0f;



	glm::mat3 permutation(0.);
	permutation[1][0] = 1.;
	permutation[2][1] = 1;
	permutation[0][2] = 1;
	// Set up vertex data (and buffer(s)) and attribute pointers
	std::vector<VertexData> verticesInit =
	{
		VertexData(glm::vec3(a, a, a),glm::vec3(1.0, 0.0,0.0),glm::vec2(0.), 0.0), // Left
		VertexData(glm::vec3(-a,-a, a), glm::vec3(1.0, 0.0, 0.0), glm::vec2(0.), 1.0),// Right
		VertexData(glm::vec3(a, -a, a), glm::vec3(1.0, 0.0, 0.0), glm::vec2(0.), 0.5),// Top
		VertexData(glm::vec3(a, a, a),glm::vec3(0.0, 1.0,0.0), glm::vec2(0.), 0.0), // Left
		VertexData(glm::vec3(-a,-a, a), glm::vec3(0.0, 1.0, 0.0), glm::vec2(0.), 1.0),// Right
		VertexData(glm::vec3(-a, a, a), glm::vec3(0.0, 1.0, 0.0), glm::vec2(0.), 0.5),// Top

		VertexData(glm::vec3(a, a, -a),glm::vec3(0.0, 0.0,1.0), glm::vec2(0.), 0.0), // Left
		VertexData(glm::vec3(-a,-a, -a), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.),1.0),// Right
		VertexData(glm::vec3(a, -a, -a), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.), 0.5),// Top
		VertexData(glm::vec3(a, a, -a),glm::vec3(1.0, 1.0,1.0), glm::vec2(0.), 0.0), // Left
		VertexData(glm::vec3(-a,-a, -a), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.), 1.0),// Right
		VertexData(glm::vec3(-a, a, -a), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.), 0.5)// Top

	};

	std::vector<VertexData> vertices(verticesInit);

	for (int i = 0; i < 2; i++)
	{
		auto vInit = verticesInit;

		for (auto& vert : vInit)
		{
			vert.m_pos = permutation * vert.m_pos;
		}

		std::move(vInit.begin(), vInit.end(), std::back_inserter(vertices));

		permutation = glm::transpose(permutation);
	}




	// pos
	ZGLStride stride1;
	stride1.m_offset = sizeof(glm::vec3),
		stride1.m_type = GL_FLOAT;
	stride1.m_size = 3;

	// color
	ZGLStride stride2;
	stride2.m_offset = sizeof(glm::vec3),
		stride2.m_type = GL_FLOAT;
	stride2.m_size = 3;

	//alpha
	ZGLStride stride3;
	stride3.m_offset = sizeof(float),
		stride3.m_type = GL_FLOAT;
	stride3.m_size = 1;

	ZGLVAODrawableParam paramDrawable;

	paramDrawable.m_stride = sizeof(VertexData);
	paramDrawable.m_nbVertex = vertices.size();
	paramDrawable.m_pVertices = (void *)&vertices[0];
	paramDrawable.m_strides = { stride1, stride2, stride3 };

	m_VAOdrawable.Init(paramDrawable);



	m_CameraMap[TRACKBALLCAMERA] = new CameraTrackBall(m_pWindowEnv->get());
	m_CameraMap[FREECAMERA] = new CameraFree(m_pWindowEnv->get());
	m_pCam = m_CameraMap[TRACKBALLCAMERA];
	
	PieceWiseBezierCurve<glm::vec2> track;
	std::vector<BezierCurve<glm::vec2>> trackCtrPt(10);
	float s = 50.0;
	for (int i=0;i<trackCtrPt.size();i++)
		trackCtrPt[i].setCtrlPt({ glm::vec2(i*s,0),
								glm::vec2((i + 0.5)*s,0),
								  glm::vec2((i + 0.5)*s,0),
								  glm::vec2((i + 1)*s,0) });
	
	track.Init(trackCtrPt);
	//std::string path = "path842";
	//std::string path = "curvelong";
	//track = PWBezierCurveParser::Parse("D:/Documents/inkscape/test.svg", path);
	m_trackDrawable.Init<float>(track.getPieces(),100);
	
	BoundingBox<glm::vec2> bbTrack = m_trackDrawable.getBoundingBox<float>();

	m_offsetScaleCurve.x = -bbTrack.getCenter<float>().x;
	m_offsetScaleCurve.y = -bbTrack.getCenter<float>().y;

	float scaleMax = 0.5 / std::max(bbTrack.getSize().x, bbTrack.getSize().y);
	m_offsetScaleCurve.z = scaleMax;
	m_offsetScaleCurve.w = scaleMax;



	float d = 1.0;
	/*PieceWiseBezierCurve<glm::vec2> section;
	std::vector<BezierCurve<glm::vec2>> sectionCtrPt(4);
	sectionCtrPt[0].setCtrlPt({ glm::vec2(-d,-d),glm::vec2(-d,0),glm::vec2(-d,0),glm::vec2(-d,d) });
	sectionCtrPt[1].setCtrlPt({ glm::vec2(-d,d),glm::vec2(0,d/2.),glm::vec2(0,d/2.),glm::vec2(d,0) });
	sectionCtrPt[2].setCtrlPt({ glm::vec2(d,0),glm::vec2(d,0),glm::vec2(d,0),glm::vec2(d,0) });
	sectionCtrPt[3].setCtrlPt({ glm::vec2(d,0),glm::vec2(0,-d / 2.),glm::vec2(0,-d / 2.),glm::vec2(-d,-d) });*/


	PieceWiseBezierCurve<glm::vec2> section;
	std::vector<BezierCurve<glm::vec2>> sectionCtrPt(3);
	sectionCtrPt[0].setCtrlPt({ glm::vec2(-d,-d),glm::vec2(-d,0),glm::vec2(-d,0),glm::vec2(-d,d) });
	sectionCtrPt[1].setCtrlPt({ glm::vec2(-d,d),glm::vec2(0,d / 2.),glm::vec2(0,d / 2.),glm::vec2(d,0) });
	sectionCtrPt[2].setCtrlPt({ glm::vec2(d,0),glm::vec2(0,-d / 2.),glm::vec2(0,-d / 2.),glm::vec2(-d,-d) });

	/*std::vector<BezierCurve<glm::vec2>> sectionCtrPt(1);
	sectionCtrPt[0].setCtrlPt({ glm::vec2(-d,-d),glm::vec2(-d,0),glm::vec2(-d,0),glm::vec2(-d,d) });*/
	
	section.Init(sectionCtrPt);

	PieceWiseBezierCurve<glm::vec2> sectionSquare;
	std::vector<BezierCurve<glm::vec2>> sectionCtrPtSquare(4);
	
	sectionCtrPtSquare[0].setCtrlPt({ glm::vec2(-d,-d),glm::vec2(-d,0),glm::vec2(-d,0),glm::vec2(-d,d) });
	sectionCtrPtSquare[1].setCtrlPt({ glm::vec2(-d,d),glm::vec2(0,d),glm::vec2(0,d),glm::vec2(d,d) });
	sectionCtrPtSquare[2].setCtrlPt({ glm::vec2(d,d),glm::vec2(d,0),glm::vec2(d,0),glm::vec2(d,-d) });
	sectionCtrPtSquare[3].setCtrlPt({ glm::vec2(d,-d),glm::vec2(0,-d),glm::vec2(0,-d),glm::vec2(-d,-d) });
	

	sectionSquare.Init(sectionCtrPtSquare);



	std::vector<PieceWiseBezierCurve<glm::vec2>> sections(track.size()+1);
	glm::mat2x2 m;
	float theta = PI / 2;
	m[0][0] = cos(theta);
	m[0][1] = sin(theta);
	m[1][0] = -sin(theta);
	m[1][1] = cos(theta);

	bool b = false;
	for (auto& sec : sections)
	{
		sec = section;
		/*if (!b)
			sec = section;
		else
			sec = sectionSquare;*/

		b = !b;
			section.Mult(m);
			//sectionSquare.Mult(m);
	}

	PieceWiseBezierSurface<glm::vec3> pwbezsurf = InitPWBezSurf(track, sections);

	

	m_CameraMap[BEZIERCAMERA] = new CameraPieceWiseBezierSurface(m_pWindowEnv->get(),pwbezsurf);
	m_pCam = m_CameraMap[BEZIERCAMERA];

	m_pwbezSurf.Init(pwbezsurf, 10, 100);


	//mapUniform[SHADER_SIZE] = UniformVar(eZGLtypeUniform::ZGL_FVEC1);
	//mapUniform[SHADER_MVP] = UniformVar(eZGLtypeUniform::ZGL_FMAT4);


	std::vector<glm::vec2> quad_vertex_buffer_data = {
		glm::vec2(0,0),
		glm::vec2(1,0),
		glm::vec2(1,1),
		glm::vec2(0,0),
		glm::vec2(0,1),
		glm::vec2(1,1)
	};

	ZGLVAODrawableParam paramDrawableQuad;
	paramDrawableQuad.m_stride = sizeof(glm::vec2);
	paramDrawableQuad.m_nbVertex = quad_vertex_buffer_data.size();
	paramDrawableQuad.m_pVertices = (void *)&quad_vertex_buffer_data[0];
	// color
	ZGLStride strideQuad;
	strideQuad.m_offset = sizeof(glm::vec2),
	strideQuad.m_type = GL_FLOAT;
	strideQuad.m_size = 2;

	paramDrawableQuad.m_strides = { strideQuad };

	m_quad.Init(paramDrawableQuad);


	MapUniform uniformMap1;
	uniformMap1[SHADER_OFFSET_SCALE]= UniformVar(eZGLtypeUniform::ZGL_FVEC4);
	uniformMap1[SHADER_USETEX] = UniformVar(eZGLtypeUniform::ZGL_IVEC1);
	m_quadShader.Init("quad", false, uniformMap1);
	m_FBO.Init(500, 500, 1);
	
	return true;
}

void MyApp::OpenGLRender()
{
	

	glClearColor(1., 1., 1., 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_FBO.BindForWriting();
	glDisable(GL_DEPTH_TEST);
	
	m_quadShader.Enable();
	
	int bUseTex = 0;
	//m_quadShader.updateUniform(SHADER_OFFSET_SCALE, (void *)&m_offsetScaleCurve);
	m_quadShader.updateUniform(SHADER_OFFSET_SCALE, (void *)&m_offsetScale);
	m_quadShader.updateUniform(SHADER_USETEX, (void *)&bUseTex);

	m_quad.Render(GL_TRIANGLE_STRIP);

	FBO::BindToScreen();
	glEnable(GL_DEPTH_TEST);
	m_pCam->Update(m_elapsedTime);
	
	m_shader[m_idShader].Enable();




	glm::mat4 MVP;

	MVP = m_pCam->getProjectionView() * m_pCam->getView();

	m_shader[m_idShader].updateUniform(SHADER_SIZE, (void *)&m_cumulTime);
	m_shader[m_idShader].updateUniform(SHADER_MVP, (void *)glm::value_ptr(MVP));
	//m_VAOdrawable.Render(GL_TRIANGLES);
	glLineWidth(1.0f);
	//m_bezierCurve.draw(GL_LINE_STRIP);
	//m_bezierSurface.draw(GL_TRIANGLE_STRIP);
	m_pwbezSurf.Draw(GL_TRIANGLE_STRIP);

	m_FBO.BindForReading(GL_TEXTURE0);
	glDisable(GL_DEPTH_TEST);
	m_quadShader.Enable();
	
	//glm::vec4 offsetScale(0, 0, 1.0, 1.0);
	bUseTex = 1;
	m_quadShader.updateUniform(SHADER_OFFSET_SCALE, (void *)&m_offsetScale);
	m_quadShader.updateUniform(SHADER_USETEX, (void *)&bUseTex);
	//m_quad.Render(GL_TRIANGLES);

	bUseTex = 0;
	m_quadShader.updateUniform(SHADER_OFFSET_SCALE, (void *)&m_offsetScaleCurve);
	m_quadShader.updateUniform(SHADER_USETEX, (void *)&bUseTex);
	//m_trackDrawable.draw(GL_LINE_STRIP);


	glEnable(GL_DEPTH_TEST);
}

void MyApp::Destroy()
{
	m_VAOdrawable.Destroy();

	m_pWindowEnv->destroy();
}

void MyApp::ImguiDraw()
{
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("MyAPP",nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("This is some useful text.");

	const char * items[] = { "TRACKBALL","FREE","BEZIER" };
	if (ImGui::Combo(" CAMERA ", &m_ctrl.m_selectedCam, items, IM_ARRAYSIZE(items)))
	{
		if (m_CameraMap[static_cast<eCameraType>(m_ctrl.m_selectedCam)] != nullptr)
		{
			Camera* pformerCam = m_pCam;
			m_pCam = m_CameraMap[static_cast<eCameraType>(m_ctrl.m_selectedCam)];

			m_pCam->Init(pformerCam->getEyePos(), pformerCam->getDirection(), pformerCam->getUp());
		}

	}
	m_pCam->ImguiDraw();
	
	ImGui::SliderFloat4("offsetSCale", &m_offsetScale[0], 0.1f, 10.0f);
	ImGui::SliderFloat4("offsetSCaleCurve", &m_offsetScaleCurve[0], 0.1f, 10.0f);

	ImGui::Checkbox(" id shader", &m_idShader);
	if (ImGui::SliderFloat("visi", &m_visi, 0.1f, 2.0f))
	{
		m_shader[1].Enable();
		m_shader[1].updateUniform(SHADER_VISI, (void *)&m_visi);
		m_shader[0].Enable();
		m_shader[0].updateUniform(SHADER_VISI, (void *)&m_visi);
	}
	ImGui::End();
}
