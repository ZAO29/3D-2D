#pragma once

#include <stdint.h>

struct GLFWwindow;

class WindowEnv
{
public:
	WindowEnv();
	~WindowEnv();

	void init(int32_t width, int32_t height, void * pApp);

	void PollEvent();

	bool IsRunning();

	void swapBuffer();

	void destroy();

	void getScreenSize();


	GLFWwindow * get() { return m_pwindow; };

private:

	void initGlew();

	GLFWwindow * m_pwindow = nullptr;
	int32_t m_windowWidth = 0;
	int32_t m_windowHeight = 0;
};

