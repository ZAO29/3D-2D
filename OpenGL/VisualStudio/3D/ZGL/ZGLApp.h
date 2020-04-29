/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZGLApp.h
 * Author: nicolas
 *
 * Created on December 5, 2019, 9:23 PM
 */

#ifndef ZGLAPP_H
#define ZGLAPP_H


#include <chrono>

#include "GL/glew.h"

#include <map>
#include "Camera.h"
#include "WindowEnv.h"

#include <vector>




class ZGLApp {
public:
    ZGLApp();
    ZGLApp(const ZGLApp& orig);
    virtual ~ZGLApp();

    
	virtual void OpenGLRender();
    virtual bool Init();
    void Run();
	virtual void Destroy();

	void KeyCallback(int key, int scancode, int action, int mods);
	
	virtual void ImguiDraw();
 
private :
	void InitImgui();
	void ImguiRender();
	void ImguiDestroy();
    
protected :     
        
    
    
    WindowEnv* m_pWindowEnv;
	Camera* m_pCam;

	std::chrono::time_point<std::chrono::system_clock> m_time;
	float m_elapsedTime;
	float m_cumulTime = 0.0;;

	bool m_bImguiRender = false;
	bool m_bfullScreen = false;
	unsigned int m_width = 1080;
	unsigned int m_height = 720;



	eCameraType m_typeCamera = eCameraType::FREECAMERA;

	std::map<eCameraType, Camera*> m_CameraMap;
	std::string m_name = "ZGLApp";
	std::vector<float> m_FPSs;
	int m_idFPS;
	

	
};

#endif /* ZGLAPP_H */

