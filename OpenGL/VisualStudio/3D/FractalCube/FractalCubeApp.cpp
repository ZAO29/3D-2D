#include "stdafx.h"
#include "FractalCubeApp.h"
#include <ZGL/CameraFree.h>


#include <gtc/type_ptr.hpp>

FractalCubeApp::FractalCubeApp()
{
}


FractalCubeApp::~FractalCubeApp()
{
}

bool FractalCubeApp::Init()
{
	//m_bfullScreen = true;
	//m_width = 1920;
	//m_height = 1080;
	m_bfullScreen = false;
	m_width = 1280;
	m_height = 720;
	ZGLApp::Init();

	float a = 1.0f;
	glm::mat3 permutation(0.);
	permutation[1][0] = 1.;
	permutation[2][1] = 1;
	permutation[0][2] = 1;
	// Set up vertex data (and buffer(s)) and attribute pointers
	std::vector<VertexData> verticesInit =
	{
		VertexData(glm::vec3(a, a, a),glm::vec3(1.0, 0.0,0.0),glm::vec2(1.), 0.0), // Left
		VertexData(glm::vec3(-a,-a, a), glm::vec3(1.0, 0.0, 0.0), glm::vec2(0), 1.0),// Right
		VertexData(glm::vec3(a, -a, a), glm::vec3(1.0, 0.0, 0.0), glm::vec2(1,0), 0.5),// Top
		VertexData(glm::vec3(a, a, a),glm::vec3(0.0, 1.0,0.0), glm::vec2(1), 0.0), // Left
		VertexData(glm::vec3(-a,-a, a), glm::vec3(0.0, 1.0, 0.0), glm::vec2(0.), 1.0),// Right
		VertexData(glm::vec3(-a, a, a), glm::vec3(0.0, 1.0, 0.0), glm::vec2(0,1), 0.5),// Top

		VertexData(glm::vec3(a, a, -a),glm::vec3(0.0, 0.0,1.0), glm::vec2(1), 0.0), // Left
		VertexData(glm::vec3(-a,-a, -a), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.),1.0),// Right
		VertexData(glm::vec3(a, -a, -a), glm::vec3(0.0, 0.0, 1.0), glm::vec2(1,0.), 0.5),// Top
		VertexData(glm::vec3(a, a, -a),glm::vec3(1.0, 1.0,1.0), glm::vec2(1.), 0.0), // Left
		VertexData(glm::vec3(-a,-a, -a), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.), 1.0),// Right
		VertexData(glm::vec3(-a, a, -a), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.,1.), 0.5)// Top

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

	ZGLStride stride3;
	stride3.m_offset = sizeof(glm::vec2),
		stride3.m_type = GL_FLOAT;
	stride3.m_size = 2;

	//alpha
	ZGLStride stride4;
	stride4.m_offset = sizeof(float),
		stride4.m_type = GL_FLOAT;
	stride4.m_size = 1;

	{
		ZGLVAODrawableParam paramDrawable;

		paramDrawable.m_stride = sizeof(VertexData);
		paramDrawable.m_nbVertex = vertices.size();
		paramDrawable.m_pVertices = (void *)&vertices[0];
		paramDrawable.m_strides = { stride1, stride2, stride3,stride4 };

		m_cube.Init(paramDrawable);
	}


	float eps = 0.02;
	std::vector<VertexData> quad_vertex_buffer_data = {
		VertexData(glm::vec3(-1.0f + eps, -1.0f + eps, 0.0f),glm::vec2(0,0)),
		VertexData(glm::vec3(1.0f - eps, -1.0f + eps, 0.0f),glm::vec2(1,0)),
		VertexData(glm::vec3(-1.0f + eps,  1.0f - eps, 0.0f),glm::vec2(0,1)),
		VertexData(glm::vec3(-1.0f + eps,  1.0f - eps, 0.0f),glm::vec2(0,1)),
		VertexData(glm::vec3(1.0f - eps, -1.0f + eps, 0.0f),glm::vec2(1,0)),
		VertexData(glm::vec3(1.0f - eps,  1.0f - eps, 0.0f),glm::vec2(1,1))
	};

	{
		ZGLVAODrawableParam paramDrawable;

		paramDrawable.m_stride = sizeof(VertexData);
		paramDrawable.m_nbVertex = quad_vertex_buffer_data.size();
		paramDrawable.m_pVertices = (void *)&quad_vertex_buffer_data[0];
		paramDrawable.m_strides = { stride1, stride2, stride3,stride4 };

		m_quad.Init(paramDrawable);
	}

	m_ptex = new Texture(0., 0., 0.);

	MapUniform listvar;
	listvar[SHADER_MVP] = UniformVar(eZGLtypeUniform::ZGL_FMAT4);
	m_shader.Init("shader", false, listvar);


	m_motionBlurShader.Init("motionblur", false, MapUniform());

	m_pCam = new CameraFree(m_pWindowEnv->get());
	m_pCam->setEyePos(glm::vec3(0, 3, 0));
	m_pCam->setDirection(glm::vec3(0, -1, 0));
	m_pCam->m_speed = 0.3f;


	m_FBO[0].Init(m_width, m_height, 1);
	m_FBO[1].Init(m_width, m_height, 1);

	m_MotionBlurFBOs.resize(m_nbMotionBlur);
	for (auto & FBO : m_MotionBlurFBOs)
	{
		FBO.Init(m_width, m_height, 1);
	}


	return true;
}

void FractalCubeApp::OpenGLRender()
{
	bool id = false;
	m_FBO[id].BindForWriting();
	//FBO::BindToScreen();
	glClearColor(1., 1., 1., 1.);
	glClear(GL_COLOR_BUFFER_BIT);

	m_shader.Enable();
	
	glm::mat4 MVP(1.);
	glDisable(GL_DEPTH_TEST);
	m_shader.updateUniform(SHADER_MVP, (void *)glm::value_ptr(MVP));
	m_ptex->Bind(GL_TEXTURE0);
	m_quad.Render(GL_TRIANGLES);

	float step = 1. / float(m_nbOctave);

	for (int i = 0; i < m_nbOctave; i++)
	{
		bool next_id = !id;
		{
			m_FBO[next_id].BindForWriting();
			m_FBO[id].BindForReading(GL_TEXTURE0);

			glClearColor(0., 0., 0., 1.);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			glEnable(GL_DEPTH_TEST);
			MVP = m_pCam->getProjectionView() * m_pCam->getView();


			m_shader.updateUniform(SHADER_MVP, (void *)glm::value_ptr(MVP));

			m_cube.Render(GL_TRIANGLES);
			id = next_id;
		}
		next_id = !id;
		m_FBO[next_id].BindForWriting();
		m_FBO[id].BindForReading(GL_TEXTURE0);

		glClearColor(1., 1., 1., 1.);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glEnable(GL_DEPTH_TEST);
		MVP = m_pCam->getProjectionView() * m_pCam->getView();

		MVP = glm::mat4(1.);
		m_shader.updateUniform(SHADER_MVP, (void *)glm::value_ptr(MVP));

		m_quad.Render(GL_TRIANGLES);
		id = next_id;
	}



	
	m_MotionBlurFBOs[m_idFBO].BindForWriting();
	glClearColor(0., 0., 0., 1.);
	glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
	m_FBO[id].BindForReading(GL_TEXTURE0);
	glEnable(GL_DEPTH_TEST);
	MVP = m_pCam->getProjectionView() * m_pCam->getView();

	
	m_shader.updateUniform(SHADER_MVP, (void *)glm::value_ptr(MVP));
	
	m_cube.Render(GL_TRIANGLES);

	FBO::BindToScreen();
	m_motionBlurShader.Enable();
	for (int i = 0; i < m_nbMotionBlur; i++)
	{
		m_MotionBlurFBOs[i].BindForReading(GL_TEXTURE0 + i);
	}
	m_MotionBlurFBOs[id].RenderQuad();
	m_idFBO++;
	m_idFBO = m_idFBO % m_nbMotionBlur;
}

void FractalCubeApp::Destroy()
{
	m_cube.Destroy();
}

void FractalCubeApp::ImguiDraw()
{
}
