#include "stdafx.h"
#include "CameraPieceWiseBezierSurface.h"
#include <ZGL/Listener.h>




CameraPieceWiseBezierSurface::CameraPieceWiseBezierSurface(GLFWwindow* pwin, PieceWiseBezierSurface<glm::vec3> surf):
	Camera(pwin),
	m_surface(surf),
	m_surfaceDerU(surf.derivateU<float>()),
	m_surfaceDerV(surf.derivateV<float>())
{
	m_speed = 0.1;
}

CameraPieceWiseBezierSurface::~CameraPieceWiseBezierSurface()
{
}

void CameraPieceWiseBezierSurface::Update(float elapsedTime)
{
	if (Listener::sgetKeyState(GLFW_KEY_A))
	{
		m_bForwardAuto = !m_bForwardAuto;
	}




	if (Listener::sgetKeyState(GLFW_KEY_UP) 
		|| m_bForwardAuto)
	{
			m_parameter.y += elapsedTime * m_speed;


	}

	if (Listener::sgetKeyState(GLFW_KEY_DOWN))
	{
		m_parameter.y -= elapsedTime * m_speed;
	}


	if (Listener::sgetKeyState(GLFW_KEY_F))
	{
		m_speed *= 1.01;


	}

	if (Listener::sgetKeyState(GLFW_KEY_G))
	{
		m_speed /= 1.01;
	}



	if (Listener::sgetKeyState(GLFW_KEY_LEFT))
	{
		m_parameter.x += elapsedTime * m_speed;

	}

	if (Listener::sgetKeyState(GLFW_KEY_RIGHT))
	{
		m_parameter.x -= elapsedTime * m_speed;
	}


	if (Listener::sgetKeyState(GLFW_KEY_R))
	{
		m_parameter = glm::vec2(0.);
	}




	m_eye = m_surface.Eval(m_parameter.x, m_parameter.y);
	m_direction = m_surfaceDerV.Eval(m_parameter.x, m_parameter.y);
	m_right = m_surfaceDerU.Eval(m_parameter.x, m_parameter.y);
	m_up = glm::normalize(glm::cross(m_right,m_direction));
	m_eye += 0.4f*m_up;


}

glm::mat4 CameraPieceWiseBezierSurface::getView()
{
	return glm::lookAt(m_eye, m_eye + m_direction, m_up);
}
