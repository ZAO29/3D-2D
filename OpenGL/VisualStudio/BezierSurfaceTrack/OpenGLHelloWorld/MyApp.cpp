#include "stdafx.h"
#include "MyApp.h"

#include "CameraFree.h"
#include "CameraTrackBall.h"

#include "imgui.h"

#include <gtc/type_ptr.hpp>

#include "Constant.h"


#define SHADER_SIZE "usize"
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

	m_shader.Init("shader", false, mapUniform);

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
	ctrlPtGrid.resize(3);
	
	for (auto & ctrlPtLine : ctrlPtGrid)
	{
		ctrlPtLine.resize(4);
	}
	float coeff1 = 3.0;
	ctrlPtGrid[0][0] = coeff1 * glm::vec3(-1., -1., -1.);
	ctrlPtGrid[0][1] = coeff1 * glm::vec3(-1., -1., 1.);
	ctrlPtGrid[0][2] = coeff1 * glm::vec3(1., -1., 1.);
	ctrlPtGrid[0][3] = coeff1 * glm::vec3(1., -1., -1.);

	ctrlPtGrid[1][0] = coeff1 * glm::vec3(-1., 0., -0.);
	ctrlPtGrid[1][1] = coeff1 * glm::vec3(-1., 0., 0.);
	ctrlPtGrid[1][2] = coeff1 * glm::vec3(1., 0., 0.);
	ctrlPtGrid[1][3] = coeff1 * glm::vec3(1., 0., 0.);

	ctrlPtGrid[2][0] = coeff1 * glm::vec3(-1., 1., -1.);
	ctrlPtGrid[2][1] = coeff1 * glm::vec3(-1., 1., -2.);
	ctrlPtGrid[2][2] = coeff1 * glm::vec3(1., 1., -2.);
	ctrlPtGrid[2][3] = coeff1 * glm::vec3(1., 1., -1.);


	m_bezierSurface.Init(ctrlPtGrid, 100, 50);

	return true;
}

void MyApp::OpenGLRender()
{
	m_pCam->Update(m_elapsedTime);
	
	m_shader.Enable();




	glm::mat4 MVP;

	MVP = m_pCam->getProjectionView() * m_pCam->getView();

	m_shader.updateUniform(SHADER_SIZE, (void *)&m_elapsedTime);
	m_shader.updateUniform(SHADER_MVP, (void *)glm::value_ptr(MVP));
	m_VAOdrawable.Render(GL_TRIANGLES);
	glLineWidth(1.0f);
	m_bezierCurve.draw(GL_LINE_STRIP);
	m_bezierSurface.draw(GL_LINE_STRIP);
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

	ImGui::Begin("MyAPP");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");

	const char * items[] = { "TRACKBALL","FREE" };
	if (ImGui::Combo(" CAMERA ", &m_ctrl.m_selectedCam, items, IM_ARRAYSIZE(items)))
	{
		if (m_CameraMap[static_cast<eCameraType>(m_ctrl.m_selectedCam)] != nullptr)
		{
			Camera* pformerCam = m_pCam;
			m_pCam = m_CameraMap[static_cast<eCameraType>(m_ctrl.m_selectedCam)];

			m_pCam->Init(pformerCam->getEyePos(), pformerCam->getDirection(), pformerCam->getUp());
		}

	}
	ImGui::End();
}
