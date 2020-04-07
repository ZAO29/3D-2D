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

#include "GL/glew.h"

#include <map>
#include "Camera.h"
#include "WindowEnv.h"
#include "Shaders.h"



class ZGLApp {
public:
    ZGLApp();
    ZGLApp(const ZGLApp& orig);
    virtual ~ZGLApp();

    
	virtual void OpenGLRender();
    virtual bool Init();
    void Run();
	void Destroy();

 
private :
       
    
protected :     
        
    std::map<eCameraType,Camera*> m_CameraMap;
    Camera* m_pCam;
    
    WindowEnv* m_pWindowEnv;
	Shader m_shader;

	unsigned int m_VAO;
	unsigned int m_VBO;

};

#endif /* ZGLAPP_H */

