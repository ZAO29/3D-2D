/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeometryShaderApp.h
 * Author: nicolas
 *
 * Created on February 27, 2020, 8:47 PM
 */

#ifndef GEOMETRYSHADERAPP_H
#define GEOMETRYSHADERAPP_H


#include <ZGL/ZGLApp.h>

#include "ZGL/FBO.h"

#include "ZGL/Texture.h"
#include  "ZGL/ZVBO.h"
#include  "ZGL/Shaders.h"





#include <ctime>











class App : public ZGLApp {
    

    

   
    
public:

    
    virtual bool Init() override;
    
    virtual void OpenGLRender() override;
    virtual void ImguiDraw() override;
    
    virtual void Destroy();
    
    void OpenGLWorldRender();
    
    
 
    
    
private:
         
    ZVBO m_quadVBO;
    Shader m_shader;



    
    
       
    



    
    
    

};

#endif /* GEOMETRYSHADERAPP_H */

