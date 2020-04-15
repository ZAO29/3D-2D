#include "stdafx.h"
#include "Listener.h"
#include <GLFW/glfw3.h>
#include <iostream>



#define LETTERTOCHAR(letter) case GLFW_KEY_##letter : return std::string(#letter);break;

Listener* Listener::s_pSingleton = nullptr;

Listener::Listener()
{
}


Listener::~Listener()
{
}

void Listener::setKeyState(unsigned int keycode, bool state)
{
	m_keyStates[keycode] = state;
}

bool Listener::getKeyState(unsigned int keycode)
{
		return m_keyStates[keycode];
}

Listener * Listener::getSingleListener()
{
	if (s_pSingleton == nullptr)
	{
		s_pSingleton = new Listener();

		s_pSingleton->m_qwertyToAzerty[GLFW_KEY_Q] = GLFW_KEY_A;
		s_pSingleton->m_qwertyToAzerty[GLFW_KEY_A] = GLFW_KEY_Q;
		s_pSingleton->m_qwertyToAzerty[GLFW_KEY_Z] = GLFW_KEY_W;
		s_pSingleton->m_qwertyToAzerty[GLFW_KEY_W] = GLFW_KEY_Z;
		s_pSingleton->m_qwertyToAzerty[GLFW_KEY_COMMA] = GLFW_KEY_M;
	}
	return s_pSingleton;
}

void Listener::sUpdateSingleListenerKey(int key, int scancode, int action, int mods)
{
	getSingleListener()->mUpdateSingleListenerKey(key, scancode, action, mods);
}


void Listener::mUpdateSingleListenerKey(int key, int scancode, int action, int mods)
{
	if (m_qwertyToAzerty.find(key) != m_qwertyToAzerty.end())
	{
		key = m_qwertyToAzerty[key];
	}
	if (action == GLFW_PRESS)
	{
		m_keyStates[key] = true;
		std::cout << "key is pressed : " << getStringKey(key) << std::endl;
	}
	if (action == GLFW_RELEASE)
	{
		m_keyStates[key] = false;
		std::cout << "key is pressed : " << getStringKey(key) << std::endl;
	}
}

void Listener::mUpdateSingleListenerMouseButton(int button, int action, int mods)
{
	bool ispressed = (action == GLFW_PRESS);
	m_mouseButtonState[button] = ispressed;


}

std::string Listener::getStringKey(int key)
{
	switch (key)
	{
		LETTERTOCHAR(A)
		LETTERTOCHAR(Z)
		LETTERTOCHAR(E)
		LETTERTOCHAR(R)
		LETTERTOCHAR(T)
		LETTERTOCHAR(Y)
		LETTERTOCHAR(U)
		LETTERTOCHAR(I)
		LETTERTOCHAR(O)
		LETTERTOCHAR(P)
		LETTERTOCHAR(Q)
		LETTERTOCHAR(S)
		LETTERTOCHAR(D)
		LETTERTOCHAR(F)
		LETTERTOCHAR(G)
		LETTERTOCHAR(H)
		LETTERTOCHAR(J)
		LETTERTOCHAR(K)
		LETTERTOCHAR(L)
		LETTERTOCHAR(M)
		LETTERTOCHAR(W)
		LETTERTOCHAR(X)
		LETTERTOCHAR(C)
		LETTERTOCHAR(V)
		LETTERTOCHAR(B)
		LETTERTOCHAR(N)
		LETTERTOCHAR(UP)
		LETTERTOCHAR(DOWN)
		LETTERTOCHAR(LEFT)
		LETTERTOCHAR(RIGHT)
		default:
			return std::string("unknown");

	}
}
