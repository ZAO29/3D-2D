#pragma once

#include <map>
#include <string>
#include <GLFW/glfw3.h>
class Listener
{
	public:

		void setKeyState(unsigned int keycode, bool state);
		

		bool getKeyState(unsigned int keycode);
		
		unsigned int getRealValue(unsigned int keycode) { if (m_qwertyToAzerty.find(keycode) != m_qwertyToAzerty.end()) return m_qwertyToAzerty[keycode]; else return keycode; }
		
		
		static unsigned int sgetRealValue(unsigned int keycode) { return getSingleListener()->getRealValue(keycode); }
		static Listener* getSingleListener();
		static std::string sgetStringKey(int key){ return getSingleListener()->getStringKey(key); }
		static void sUpdateSingleListenerKey(int key, int scancode, int action, int mods);
		static void sUpdateSingleListenerMouseButton(int button, int action, int mods) { getSingleListener()->mUpdateSingleListenerMouseButton(button, action, mods); }

		static bool sgetKeyState(int keycode) { return getSingleListener()->getKeyState(keycode); }
		static bool sgetMouseButtonState(int button) { return getSingleListener()->m_mouseButtonState[button]; }

private:
	Listener();
	~Listener();
	
	void mUpdateSingleListenerKey(int key, int scancode, int action, int mods);
	void mUpdateSingleListenerMouseButton(int button, int action, int mods);

	std::string getStringKey(int key);

	static Listener * s_pSingleton;

	std::map<int, int> m_qwertyToAzerty;
	std::map<unsigned int, bool> m_keyStates; // boolean for true if the key is pressed

	std::map<int, bool> m_mouseButtonState;
};

