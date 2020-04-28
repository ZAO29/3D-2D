#include "stdafx.h"
#include "WindowEnv.h"
#include <iostream>


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include "ZGLApp.h"
#include "Listener.h"
#include "Debugging.h"


void error_callback(int error, const char* description)
{
	std::cout <<"ERROR : "<<description<<std::endl;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	//if (key == GLFW_KEY_E && action == GLFW_PRESS)
		//activate_airship();

	ZGLApp* pApp = reinterpret_cast<ZGLApp*>(glfwGetWindowUserPointer(window));

	if(pApp != nullptr)
		pApp->KeyCallback(key, scancode, action, mods);
}


void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
	Listener::sUpdateSingleListenerMouseButton(button, action, mods);
	
}


WindowEnv::WindowEnv()
{
}


WindowEnv::~WindowEnv()
{
}

void WindowEnv::init(int32_t width, int32_t height, bool bFullScreen,void * pApp, std::string name)
{

	if (nullptr != m_pwindow)
	{
		std::cout << __FUNCTION__ << " window alreadey created " << std::endl;
		glfwTerminate();

		INTERNALERROR("failed to create window");
	}


	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // allow debugging
	
	// Create a GLFWwindow object that we can use for GLFW's functions
	if (bFullScreen)
	{
		m_pwindow = glfwCreateWindow(1920, 1080, name.c_str(), glfwGetPrimaryMonitor(), nullptr);
	}
	else
	{
		m_pwindow = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	}


	/*------  CALLBACK  ------*/
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(m_pwindow,key_callback);
	glfwSetMouseButtonCallback(m_pwindow, mousebutton_callback);
	
	// set the App as to pointer in order to have access to app data
	// in the different callback
	glfwSetWindowUserPointer(m_pwindow, pApp);

	if (nullptr == m_pwindow)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		INTERNALERROR("failed to create window");
	}

	glfwMakeContextCurrent(m_pwindow);

	initGlew();
}

void WindowEnv::PollEvent()
{
	glfwPollEvents();
}

bool WindowEnv::IsRunning()
{
	return !(glfwWindowShouldClose(m_pwindow));
}

void WindowEnv::swapBuffer()
{
	glfwSwapBuffers(m_pwindow);
}

void WindowEnv::destroy()
{
}

void WindowEnv::initGlew()
{
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(m_pwindow, &screenWidth, &screenHeight);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		INTERNALERROR("Failed to initialize GLEW");
	}

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);
}
