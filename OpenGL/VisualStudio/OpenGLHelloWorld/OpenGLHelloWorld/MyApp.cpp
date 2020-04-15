#include "stdafx.h"
#include "MyApp.h"

#include "CameraFree.h"
#include "CameraTrackBall.h"

#include "imgui.h"

#include <gtc/type_ptr.hpp>

#define SHADER_SIZE "usize"
#define SHADER_MVP "uMVP"

struct VertexData
{

	VertexData(glm::vec3 pos, glm::vec3 color, float alpha) :
		m_pos(pos),
		m_color(color),
		m_alpha(alpha)
	{}
	//VertexData(float x, float y, float z) :m_pos(x,y,z) {}

	glm::vec3 m_pos;
	glm::vec3 m_color;
	float m_alpha;
};





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
