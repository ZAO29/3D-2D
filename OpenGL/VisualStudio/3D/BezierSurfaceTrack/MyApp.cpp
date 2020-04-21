#include "stdafx.h"
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

	m_shader.Init("shader", true, mapUniform);

	float a = 1.0f;



	glm::mat3 permutation(0.);
	permutation[1][0] = 1.;
	permutation[2][1] = 1;
	permutation[0][2] = 1;
	// Set up vertex data (and buffer(s)) and attribute pointers
	std::vector<VertexData> verticesInit =
	{
		VertexData(glm::vec3(a, a, a),glm::vec3(1.0, 0.0,0.0), 0.0), // Left
		VertexData(glm::vec3(-a,-a, a), glm::vec3(1.0, 0.0, 0.0), 1.0),// Right
		VertexData(glm::vec3(a, -a, a), glm::vec3(1.0, 0.0, 0.0), 0.5),// Top
		VertexData(glm::vec3(a, a, a),glm::vec3(0.0, 1.0,0.0), 0.0), // Left
		VertexData(glm::vec3(-a,-a, a), glm::vec3(0.0, 1.0, 0.0), 1.0),// Right
		VertexData(glm::vec3(-a, a, a), glm::vec3(0.0, 1.0, 0.0), 0.5),// Top

		VertexData(glm::vec3(a, a, -a),glm::vec3(0.0, 0.0,1.0), 0.0), // Left
		VertexData(glm::vec3(-a,-a, -a), glm::vec3(0.0, 0.0, 1.0), 1.0),// Right
		VertexData(glm::vec3(a, -a, -a), glm::vec3(0.0, 0.0, 1.0), 0.5),// Top
		VertexData(glm::vec3(a, a, -a),glm::vec3(1.0, 1.0,1.0), 0.0), // Left
		VertexData(glm::vec3(-a,-a, -a), glm::vec3(1.0, 1.0, 1.0), 1.0),// Right
		VertexData(glm::vec3(-a, a, -a), glm::vec3(1.0, 1.0, 1.0), 0.5)// Top

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
	float coeff = 0.25;
	std::vector<glm::vec3> ctrlPt = { coeff * glm::vec3(0,0,-50), coeff * glm::vec3(0,50,-50), coeff * glm::vec3(50,0,50),coeff * glm::vec3(0,0,50) };
	m_bezierCurve.Init(ctrlPt, 100);


	std::vector<std::vector<glm::vec3>> ctrlPtGrid;
	ctrlPtGrid.resize(4);
	
	for (auto & ctrlPtLine : ctrlPtGrid)
	{
		ctrlPtLine.resize(4);
	}
	float coeff1 = 3.0;
	ctrlPtGrid[0][0] = coeff1 * glm::vec3(-1., -1., -1.);
	ctrlPtGrid[0][1] = coeff1 * glm::vec3(-0.5, -1., -1);
	ctrlPtGrid[0][2] = coeff1 * glm::vec3(0.5, -1., -1.);
	ctrlPtGrid[0][3] = coeff1 * glm::vec3(1., -1., -1.);

	ctrlPtGrid[1][0] = coeff1 * glm::vec3(-1., 0., -1.);
	ctrlPtGrid[1][1] = coeff1 * glm::vec3(-0.5, 0., -1.);
	ctrlPtGrid[1][2] = coeff1 * glm::vec3(0.5, 0., -1.);
	ctrlPtGrid[1][3] = coeff1 * glm::vec3(1., 0., -1.);

	ctrlPtGrid[2][0] = coeff1 * glm::vec3(-1., 1., 1.);
	ctrlPtGrid[2][1] = coeff1 * glm::vec3(-0.5, 1., 1.);
	ctrlPtGrid[2][2] = coeff1 * glm::vec3(0.5, 1., 1.);
	ctrlPtGrid[2][3] = coeff1 * glm::vec3(1., 1., 1.);

	ctrlPtGrid[3][0] = coeff1 * glm::vec3(-1., 2., 1.);
	ctrlPtGrid[3][1] = coeff1 * glm::vec3(-0.5, 2., 1.);
	ctrlPtGrid[3][2] = coeff1 * glm::vec3(0.5, 2., 1.);
	ctrlPtGrid[3][3] = coeff1 * glm::vec3(1., 2., 1.);





	/*ctrlPtGrid[0][0] = coeff1 * glm::vec3(-1., -1., -1.);
	ctrlPtGrid[0][1] = coeff1 * glm::vec3(0., -1., -1);
	ctrlPtGrid[0][2] = coeff1 * glm::vec3(0, -1., -1);
	ctrlPtGrid[0][3] = coeff1 * glm::vec3(1., -1., -1.);

	ctrlPtGrid[1][0] = coeff1 * glm::vec3(-1., 0., -0.);
	ctrlPtGrid[1][1] = coeff1 * glm::vec3(0., 0., 0.);
	ctrlPtGrid[1][2] = coeff1 * glm::vec3(0., 0., 0.);
	ctrlPtGrid[1][3] = coeff1 * glm::vec3(1., 0., 0.);

	ctrlPtGrid[2][0] = coeff1 * glm::vec3(-1., 1., -1.);
	ctrlPtGrid[2][1] = coeff1 * glm::vec3(0., 1., -2.);
	ctrlPtGrid[2][2] = coeff1 * glm::vec3(0., 1., -2.);
	ctrlPtGrid[2][3] = coeff1 * glm::vec3(-1., 1., -1.);*/


	m_bezierSurface.Init(ctrlPtGrid, 100, 500);


	

	PieceWiseBezierCurve<glm::vec2> track;
	/*std::vector<BezierCurve<glm::vec2>> trackCtrPt(2);
	float s = 5.0;
	trackCtrPt[0].setCtrlPt({ glm::vec2(-s,-s),glm::vec2(-s,0),glm::vec2(-s,0),glm::vec2(0,0) });
	trackCtrPt[1].setCtrlPt({ glm::vec2(0,0),glm::vec2(s,0),glm::vec2(s,0),glm::vec2(s,s) });
	track.Init(trackCtrPt);*/
	std::string path = "path839";
	//std::string path = "curvelong";
	track = PWBezierCurveParser::Parse("D:/Documents/inkscape/test.svg", path);

	PieceWiseBezierCurve<glm::vec2> section;
	std::vector<BezierCurve<glm::vec2>> sectionCtrPt(3);
	float d = 1.0;
	sectionCtrPt[0].setCtrlPt({ glm::vec2(-d,-d),glm::vec2(-d,0),glm::vec2(-d,0),glm::vec2(-d,d) });
	sectionCtrPt[1].setCtrlPt({ glm::vec2(-d,d),glm::vec2(0,d/2.),glm::vec2(0,d/2.),glm::vec2(d,0) });
	sectionCtrPt[2].setCtrlPt({ glm::vec2(d,0),glm::vec2(0,-d / 2.),glm::vec2(0,-d / 2.),glm::vec2(-d,-d) });

	section.Init(sectionCtrPt);

	std::vector<PieceWiseBezierCurve<glm::vec2>> sections(track.size()+1);
	for (auto& sec : sections)
	{
		sec = section;
	}

	PieceWiseBezierSurface<glm::vec3> pwbezsurf = InitPWBezSurf(track, sections);

	m_CameraMap[BEZIERCAMERA] = new CameraPieceWiseBezierSurface(m_pWindowEnv->get(),pwbezsurf);
	m_pCam = m_CameraMap[BEZIERCAMERA];

	m_pwbezSurf.Init(pwbezsurf, 3, 100);


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

	m_quadShader.Init("quad", false, MapUniform());
	m_FBO.Init(500, 500, 1);

	return true;
}

void MyApp::OpenGLRender()
{
	m_FBO.BindForWriting();
	glClearColor(1., 0., 0., 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	FBO::BindToScreen();

	m_pCam->Update(m_elapsedTime);
	
	m_shader.Enable();




	glm::mat4 MVP;

	MVP = m_pCam->getProjectionView() * m_pCam->getView();

	m_shader.updateUniform(SHADER_SIZE, (void *)&m_cumulTime);
	m_shader.updateUniform(SHADER_MVP, (void *)glm::value_ptr(MVP));
	//m_VAOdrawable.Render(GL_TRIANGLES);
	glLineWidth(1.0f);
	//m_bezierCurve.draw(GL_LINE_STRIP);
	//m_bezierSurface.draw(GL_TRIANGLE_STRIP);
	m_pwbezSurf.Draw(GL_TRIANGLE_STRIP);

	m_FBO.BindForReading(GL_TEXTURE0);
	glDisable(GL_DEPTH_TEST);
	m_quadShader.Enable();
	m_quad.Render(GL_TRIANGLES);

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
	
	ImGui::End();
}
