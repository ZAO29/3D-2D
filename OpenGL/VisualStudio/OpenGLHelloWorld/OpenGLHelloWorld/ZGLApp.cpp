/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZGLApp.cpp
 * Author: nicolas
 * 
 * Created on December 5, 2019, 9:23 PM
 */

#include "stdafx.h"

#include <gtc/type_ptr.hpp>

#include "ZGLApp.h"
#include "CameraFree.h"
#include "CameraTrackBall.h"

#include "GLFW/glfw3.h"
#include "Listener.h"


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


//(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void GLAPIENTRY GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{    
        std::string sType;
    switch(type)
    {
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        sType = "DEPRECATED";
        break;
    case GL_DEBUG_TYPE_ERROR:
        sType = "ERROR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        sType = "UNDEFINED";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        sType = "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        sType = "PORTABILITY";
        break;
    default:
        return;
        sType = "OTHER";
        break;
    }
    std::cout << "GL_DEBUG "<<sType.c_str()<<" : "<<message<<std::endl;
}



ZGLApp::ZGLApp()
{
}

ZGLApp::ZGLApp(const ZGLApp& orig)
{
}

ZGLApp::~ZGLApp()
{
}


void ZGLApp::Run()
{
	while (m_pWindowEnv->IsRunning())
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		m_pWindowEnv->PollEvent();
		m_pCam->updateKeyControl();
		glClearColor(0., 0., 0., 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		OpenGLRender();


		m_pWindowEnv->swapBuffer();

	}
}

void ZGLApp::Destroy()
{
	m_VAOdrawable.Destroy();

	// Terminate GLFW, clearing any resources allocated by GLFW.
	m_pWindowEnv->destroy();
}

void ZGLApp::KeyCallback(int key, int scancode, int action, int mods)
{

	Listener* s_plistener = Listener::getSingleListener();
	Listener::sUpdateSingleListener(key, scancode, action, mods);

	key = Listener::sgetRealValue(key);
	if (action == GLFW_PRESS)
	{
		std::cout <<__FUNCTION__<< "key is pressed : " << Listener::sgetStringKey(key) << std::endl;
	}
	if (action == GLFW_RELEASE)
	{
		std::cout <<__FUNCTION__<< "key is released : " << Listener::sgetStringKey(key) << std::endl;
	}
	
	//m_pCam->KeyCallback(key,scancode,action,mods);
	//Listener::sUpdateSingleListener(key, scancode, action, mods);
	


}





bool ZGLApp::Init()
{
	m_begin = std::chrono::system_clock::now();
    //m_pWindowEnv = WindowEnv::createWindowEnv(1280,720);
    
	m_pWindowEnv = new WindowEnv();

    m_pWindowEnv->init(1280,720,(void *) this);

	m_qwertyToAzerty[GLFW_KEY_Q] = GLFW_KEY_A;
	m_qwertyToAzerty[GLFW_KEY_A] = GLFW_KEY_Q;
	m_qwertyToAzerty[GLFW_KEY_Z] = GLFW_KEY_W;
	m_qwertyToAzerty[GLFW_KEY_W] = GLFW_KEY_Z;
    m_qwertyToAzerty[GLFW_KEY_COMMA] = GLFW_KEY_M;
    
    // MESSAGE ERREUR OPENGL
	glEnable(GL_DEBUG_OUTPUT);
	GLDEBUGPROC foncteur = GLErrorCallback;
    glDebugMessageCallback(foncteur,NULL);
    
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
	paramDrawable.m_pVertices = (void *) &vertices[0];
	paramDrawable.m_strides = { stride1, stride2, stride3};

	m_VAOdrawable.Init(paramDrawable);



    m_CameraMap[TRACKBALLCAMERA]= new CameraTrackBall();
    m_CameraMap[FREECAMERA]= new CameraFree();
    m_pCam = m_CameraMap[TRACKBALLCAMERA];
    
    return true;
            
}




void ZGLApp::OpenGLRender()
{
	m_shader.Enable();

	std::chrono::time_point<std::chrono::system_clock> clock;
	clock = std::chrono::system_clock::now();

	std::chrono::duration<float> elapsed_seconds = clock - m_begin;
	float t = cos(elapsed_seconds.count());


	glm::mat4 MVP;

	MVP = m_pCam->getProjectionView() * m_pCam->getView();

	m_shader.updateUniform(SHADER_SIZE, (void *)&t);
	m_shader.updateUniform(SHADER_MVP, (void *)glm::value_ptr(MVP));
	m_VAOdrawable.Render(GL_TRIANGLES);
	
}





