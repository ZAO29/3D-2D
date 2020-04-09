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
#include "Shaders.h"
#include "ZGLDrawable.h"



class ZGLApp {
public:
    ZGLApp();
    ZGLApp(const ZGLApp& orig);
    virtual ~ZGLApp();

    
	virtual void OpenGLRender();
    virtual bool Init();
    void Run();
	void Destroy();

	void KeyCallback(int key, int scancode, int action, int mods);
	

 
private :
       
    
protected :     
        
    std::map<eCameraType,Camera*> m_CameraMap;
    Camera* m_pCam;
    
    WindowEnv* m_pWindowEnv;
	Shader m_shader;
	ZGLVAODrawable m_VAOdrawable;

	std::chrono::time_point<std::chrono::system_clock> m_begin;
	

	

};

#endif /* ZGLAPP_H */

