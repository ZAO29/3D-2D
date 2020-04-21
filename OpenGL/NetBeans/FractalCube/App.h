/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Application.h
 * Author: nicolas
 *
 * Created on September 18, 2019, 3:08 PM
 */

#ifndef APP_H
#define APP_H

#include <ZGL/ZGLApp.h>

#include "BasicShader.h"
#include <ZGL/QuadShader.h>
#include "ZGL/FBO.h"

#include "ZGL/Texture.h"
#include  "ZGL/ZVBO.h"





#include <ctime>




struct Ctrl
{
    bool m_useNormalMap = true;
    bool m_rotate = true;
    bool m_debugCube = true;
    bool m_EnableBloomSFX = false;
    
};






class App : public ZGLApp {
    

    

   
    
public:
    //Application();
    //Application(const Application& orig);
    //virtual ~App();
    
    virtual bool Init() override;
    
    virtual void OpenGLRender() override;
    virtual void ImguiDraw() override;
    
    virtual void Destroy();
    
    void OpenGLWorldRender();
    
    
 
    
    
private:
     


  
    std::vector<FBO*> m_pingpongFBOs;
    
    
    ZVBO m_quadVBO;
    ZVBO m_cubeVBO;
    BasicShader* m_pShader;
    QuadShader* m_pQuadShader;
    Texture* m_pTexBlack;
    
    
       
    



    
    
    

};

#endif /* APP_H */

