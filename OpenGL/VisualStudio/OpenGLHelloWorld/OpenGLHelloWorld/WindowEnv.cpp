#include "stdafx.h"
#include "WindowEnv.h"
#include <iostream>


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

WindowEnv::WindowEnv()
{
}


WindowEnv::~WindowEnv()
{
}

void WindowEnv::init(int32_t width, int32_t height)
{

	if (nullptr != m_pwindow)
	{
		std::cout << __FUNCTION__ << " window alreadey created " << std::endl;
		glfwTerminate();

		throw std::runtime_error("failed to create window");
	}


	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	m_pwindow = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);



	if (nullptr == m_pwindow)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		throw std::runtime_error("failed to create window");
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
		throw std::runtime_error("Failed to initialize GLEW");
	}

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);
}
