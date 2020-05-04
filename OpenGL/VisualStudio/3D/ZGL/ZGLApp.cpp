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

#include <algorithm>

#include "ZGLApp.h"


#include "GLFW/glfw3.h"
#include "Listener.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Debugging.h"

#include "CameraTrackBall.h"
#include "CameraFree.h"




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
        sType = "OTHER";
        break;
    }

    std::cout << "GL_DEBUG "<<sType.c_str()<<" : "<<message<<std::endl;
	if(type == GL_DEBUG_TYPE_ERROR)
		INTERNALERROR("OPENGL ERROR");

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
	bool bMustClose = false;

	while (m_pWindowEnv->IsRunning() && !bMustClose)
	{
		bMustClose = Listener::sgetKeyState(GLFW_KEY_ESCAPE);
		
		
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		m_pWindowEnv->PollEvent();
		std::chrono::time_point<std::chrono::system_clock> clock;
		clock = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed_seconds = clock - m_time;
		m_time = clock;
		if (m_bfixedTime)
			m_elapsedTime = m_timestep;
		else
		m_elapsedTime = elapsed_seconds.count();
		
		m_cumulTime += m_elapsedTime;

		
		if (!m_bImguiRender)
			m_pCam->Update(m_elapsedTime);

		m_FPSs[m_idFPS] = 1. / m_elapsedTime;
		m_idFPS++;
		m_idFPS = m_idFPS % m_FPSs.size();

		glClearColor(0., 0., 0., 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		OpenGLRender();

		PostProcess();

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
	ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);                          // Create a window called "Hello, world!" and append into it.
	if(ImGui::Checkbox("WIREFRAME", &m_bwireframe))
	{
		if (m_bwireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	const char * items[] = { "TRACKBALL","FREE","BEZIER" };
	int camid = m_typeCamera;
	if (ImGui::Combo(" CAMERA ", &camid, items, IM_ARRAYSIZE(items)))
	{
		if (m_CameraMap[static_cast<eCameraType>(camid)] != nullptr)
		{
			Camera* pformerCam = m_pCam;
			m_pCam = m_CameraMap[static_cast<eCameraType>(camid)];
			m_typeCamera = static_cast<eCameraType>(camid);
			m_pCam->Init(pformerCam->getEyePos(), pformerCam->getDirection(), pformerCam->getUp(),pformerCam->m_speed);
		}

	}
	m_pCam->ImguiDraw();

	ImGui::PlotLines("FPS", &m_FPSs[0], m_FPSs.size());
	ImGui::Text("FPS max : %f", *std::max_element(m_FPSs.begin(),m_FPSs.end()));
	ImGui::Text("FPS min : %f", *std::min_element(m_FPSs.begin(), m_FPSs.end()));
	ImGui::Checkbox("Fixed Time", &m_bfixedTime);

	ImGui::End();
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
		INTERNALERROR("IMGUI : Failed to initialize OpenGL loader!\n");
		
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
	m_FPSs.resize(100);

	m_time = std::chrono::system_clock::now();

	m_pWindowEnv = new WindowEnv();

    m_pWindowEnv->init(m_width,m_height,m_bfullScreen,(void *) this, m_name);
	//m_pWindowEnv->init(1920, 1080, (void *)this);

    // MESSAGE ERREUR OPENGL
	glEnable(GL_DEBUG_OUTPUT);
	GLDEBUGPROC foncteur = GLErrorCallback;
    glDebugMessageCallback(foncteur,NULL);
    

	InitImgui();


	m_CameraMap[TRACKBALLCAMERA] = new CameraTrackBall(m_pWindowEnv->get());
	m_CameraMap[FREECAMERA] = new CameraFree(m_pWindowEnv->get());
	m_pCam = m_CameraMap[m_typeCamera];
	if (m_pCam == nullptr)
	{
		INTERNALERROR(" nullptr camera");
	}


	return true;
            
}




void ZGLApp::OpenGLRender()
{
	
	
}





