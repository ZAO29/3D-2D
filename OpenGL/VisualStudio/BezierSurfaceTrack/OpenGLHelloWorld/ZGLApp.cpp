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


#include "GLFW/glfw3.h"
#include "Listener.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"






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
	throw std::runtime_error("OPENGL ERROR");

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

		std::chrono::time_point<std::chrono::system_clock> clock;
		clock = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed_seconds = clock - m_time;
		m_time = clock;
		m_elapsedTime = cos(elapsed_seconds.count());
		

		glClearColor(0., 0., 0., 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		OpenGLRender();
		if (m_bImguiRender)
			ImguiRender();
		

		m_pWindowEnv->swapBuffer();

	}
}

void ZGLApp::Destroy()
{

	// Terminate GLFW, clearing any resources allocated by GLFW.
	m_pWindowEnv->destroy();
}

void ZGLApp::KeyCallback(int key, int scancode, int action, int mods)
{

	Listener* s_plistener = Listener::getSingleListener();
	Listener::sUpdateSingleListenerKey(key, scancode, action, mods);

	key = Listener::sgetRealValue(key);
	if (action == GLFW_PRESS)
	{
		std::cout <<__FUNCTION__<< "key is pressed : " << Listener::sgetStringKey(key) << std::endl;
	}
	if (action == GLFW_RELEASE)
	{
		std::cout <<__FUNCTION__<< "key is released : " << Listener::sgetStringKey(key) << std::endl;
	}


	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_F1)
		{
			m_bImguiRender = !m_bImguiRender;
			std::cout << " imgui render " << m_bImguiRender << std::endl;
		}
	}
	

	


}

void ZGLApp::ImguiDraw()
{
	
}

void ZGLApp::InitImgui()
{
	const char* glsl_version = "#version 450";

	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
	bool err = false;
	glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
	bool err = false;
	glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if (err)
	{
		throw std::runtime_error("IMGUI : Failed to initialize OpenGL loader!\n");
		
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_pWindowEnv->get(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void ZGLApp::ImguiRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImguiDraw();
		

		
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ZGLApp::ImguiDestroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}





bool ZGLApp::Init()
{
	m_time = std::chrono::system_clock::now();

	m_pWindowEnv = new WindowEnv();

    m_pWindowEnv->init(1280,720,(void *) this);

    // MESSAGE ERREUR OPENGL
	glEnable(GL_DEBUG_OUTPUT);
	GLDEBUGPROC foncteur = GLErrorCallback;
    glDebugMessageCallback(foncteur,NULL);
    

	InitImgui();

	return true;
            
}




void ZGLApp::OpenGLRender()
{
	
	
}





