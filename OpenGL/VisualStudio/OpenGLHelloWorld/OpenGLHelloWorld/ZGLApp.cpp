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

#include "ZGLApp.h"
#include "CameraFree.h"
#include "CameraTrackBall.h"

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

		glClearColor(1., 0., 0., 1);
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
	if (m_pCam != nullptr)
	{
		m_pCam->KeyCallback(key,scancode,action,mods);
	}
}


bool ZGLApp::Init()
{
    //m_pWindowEnv = WindowEnv::createWindowEnv(1280,720);
    
	m_pWindowEnv = new WindowEnv();

    m_pWindowEnv->init(1280,720,(void *) this);

    
    // MESSAGE ERREUR OPENGL
	glEnable(GL_DEBUG_OUTPUT);
	GLDEBUGPROC foncteur = GLErrorCallback;
    glDebugMessageCallback(foncteur,NULL);
    
    

	m_shader.Init("shader", false);


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f, // Left
		0.5f, -0.5f, 0.0f, // Right
		0.0f,  0.5f, 0.0f  // Top
	};


	ZGLStride stride1;
	stride1.m_offset = 3 * sizeof(GLfloat),
	stride1.m_type = GL_FLOAT;
	stride1.m_size = 3;

	ZGLVAODrawableParam paramDrawable;

	paramDrawable.m_stride = 3 * sizeof(GLfloat);
	paramDrawable.m_nbVertex = 3;
	paramDrawable.m_pVertices = (void *) &vertices[0];
	paramDrawable.m_strides = { stride1 };

	m_VAOdrawable.Init(paramDrawable);



    m_CameraMap[TRACKBALLCAMERA]= new CameraTrackBall();
    m_CameraMap[FREECAMERA]= new CameraFree();
    m_pCam = m_CameraMap[TRACKBALLCAMERA];
    
    return true;
            
}




void ZGLApp::OpenGLRender()
{
	m_shader.Enable();

	m_VAOdrawable.Render(GL_TRIANGLES);
	
}





