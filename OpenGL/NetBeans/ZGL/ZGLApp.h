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
#include "MyConstant.h"
#include "Camera.h"
#include "WindowEnv.h"

class ZGLApp {
public:
    ZGLApp();
    ZGLApp(const ZGLApp& orig);
    virtual ~ZGLApp();

    
    virtual void OpenGLRender(){}
    virtual bool Init();
    virtual void processEvents();
    virtual void ImguiDraw(){}
    bool Run();
    void Destroy();
 
private :
    void InitImgui();
    void ImguiRender();
       
    
protected :     
        
    std::map<eCameraType,Camera*> m_CameraMap;
    Camera* m_pCam;
    
    WindowEnv* m_pWindowEnv;
    bool m_bImguiCtrl = false;
    eState m_state;
};

#endif /* ZGLAPP_H */

