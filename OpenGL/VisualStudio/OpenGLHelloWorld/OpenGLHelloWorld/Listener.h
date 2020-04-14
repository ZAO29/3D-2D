#pragma once

#include <map>
#include <string>

class Listener
{
	public:

		void setKeyState(unsigned int keycode, bool state);

		bool getKeyState(unsigned int keycode);
		
		unsigned int getRealValue(unsigned int keycode) { if (m_qwertyToAzerty.find(keycode) != m_qwertyToAzerty.end()) return m_qwertyToAzerty[keycode]; else return keycode; }
		
		
		static unsigned int sgetRealValue(unsigned int keycode) { return getSingleListener()->getRealValue(keycode); }
		static Listener* getSingleListener();
		static std::string sgetStringKey(int key){ return getSingleListener()->getStringKey(key); }
		static void sUpdateSingleListener(int key, int scancode, int action, int mods);

		static bool sgetKeyState(int keycode) { return getSingleListener()->getKeyState(keycode); }

private:
	Listener();
	~Listener();
	std::map<unsigned int, bool> m_keyStates; // boolean for true if the key is pressed
	void mUpdateSingleListener(int key, int scancode, int action, int mods);

	std::string getStringKey(int key);

	static Listener * s_pSingleton;

	std::map<int, int> m_qwertyToAzerty;
};

