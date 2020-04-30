#include "stdafx.h"

#include <ZGL/imgui/imgui.h>



#include "RecordableApp.h"


#include <ZGL/Listener.h>



RecordableApp::RecordableApp()
{
}


RecordableApp::~RecordableApp()
{
}

bool RecordableApp::Init()
{
	ZGLApp::Init();


	m_shaderQuad.Init("../RecordableApp/QuadShader", false, MapUniform());

	m_captureIm.Init(m_width, m_height);
	m_captureVideo.Init(m_width, m_height, std::string("D:\\OUTPUT\\") + m_name + std::string(".avi"));
	return true;
}

void RecordableApp::PostProcess()
{
	if (m_bImageCapture)
	{
		m_captureIm.Snapshot(std::string("D:\\OUTPUT\\") + m_name, std::string("jpg"));
		m_bImageCapture = false;
	}

	if (m_bRecord)
	{
		m_captureVideo.Snapshot();
		m_captureVideo.BindForReading();
		FBO::BindToScreen();
		m_shaderQuad.Enable();
		m_captureVideo.RenderQuad();

	}

	if (m_bendRecord)
	{
		m_bRecord = false;
		m_captureVideo.End();
		m_bendRecord = false;
	}
}

void RecordableApp::KeyCallback(int key, int scancode, int action, int mods)
{
	ZGLApp::KeyCallback(key, scancode, action, mods);
	
	key = Listener::sgetRealValue(key);
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_Q)
		{
			m_bRecord = true;
		}

		if (key == GLFW_KEY_S)
		{
			m_bRecord = false;
			m_bendRecord = true;
		}
	}
}

void RecordableApp::ImguiDraw()
{
	ZGLApp::ImguiDraw();

	ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("screenshot"))
	{
		m_bImageCapture = true;
	}
	ImGui::End();
}

void RecordableApp::setTargetRender()
{
	if (!m_bImageCapture && !m_bRecord)
		FBO::BindToScreen();
	else
	{
		if (m_bImageCapture)
			m_captureIm.BindForWriting();
		else
			m_captureVideo.BindForWriting();
	}
}
