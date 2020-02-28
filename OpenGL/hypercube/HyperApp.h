/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HyperApp.h
 * Author: nicolas
 *
 * Created on December 17, 2019, 8:25 PM
 */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HyperApplication.h
 * Author: nicolas
 *
 * Created on September 18, 2019, 3:08 PM
 */

#ifndef APP_H
#define APP_H

#include <ZGL/ZGLApp.h>


#include <ZGL/QuadShader.h>
#include "ZGL/FBO.h"

#include "ZGL/Texture.h"
#include "ZGL/ZVBO.h"
#include "ZVertexIdBO.h"

#include "BasicShader.h"






#include <ctime>




struct Ctrl
{
    bool m_useNormalMap = true;
    bool m_rotate = true;
    bool m_debugCube = true;
    bool m_EnableBloomSFX = false;
    
};






class HyperApp : public ZGLApp {
    

    

   
    
public:

    
    virtual bool Init() override;
    
    virtual void OpenGLRender() override;
    virtual void ImguiDraw()override{};
    
    virtual void Destroy(){};
    
    void OpenGLWorldRender();
    
    
 
    
    
private:
     


  
    
    FBO  m_FBO;
    
    ZVBO m_quadVBO;
    ZVertex4BO m_hypercubeVBO;
    QuadShader* m_pQuadShader;
    BasicShader* m_pHyperCubeShader;
    Texture* m_pTexBlack;
    
    
       
    



    
    
    

};

#endif /* APP_H */



