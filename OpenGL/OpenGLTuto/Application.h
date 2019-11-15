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

#ifndef APPLICATION_H
#define APPLICATION_H



#include "Mesh.h"

//#include "WindowEnv.h"
#include "CameraTrackBall.h"

#include "LightShader.h"
#include "FBO.h"

#include "Texture.h"
#include "Model.h"


#include "glm.hpp"
#include "UIHandler.h"


#include <ctime>

#include "SFXBloom.h"
#include "WindowEnv.h"



struct Ctrl
{
    bool m_imguiCtrl = false;
    bool m_useNormalMap = true;
    bool m_rotate = true;
    bool m_debugCube = true;
    bool m_EnableBloomSFX = false;
    
};

struct ModelMesh
{
    Mesh* m_pMesh;
    Model m_model;
};




class Application {
    
    friend class UIHandler;
    
    enum eState
    {
        NOT_INIT,
        INITIALIZED,
        RUNNING,
        TOCLOSE,
        CLOSE
        
    };
    
    enum eEvent2Process
    {
        eEVENTCAMERA,
        eEVENTLIGHT,
        eEVENTMODEL
    };
   
    
public:
    Application();
    //Application(const Application& orig);
    virtual ~Application();
    
    bool Init();
    
    bool Run();
    
    
    
    void Destroy();
    
    
private:
    
    void processEvents();
    
    void UIEvents(XEvent event);
    
    void OnLightEvent(XEvent event);
    
    void OnModelEvent(XEvent event);
    
    //WindowEnv* m_pWindowEnv;
    GLuint m_VBO;
    GLuint m_IBO;
    GLuint m_tex;
    eState m_state;
    
    
    glm::mat4 m_MVP;


    unsigned int m_nbVertices;
    
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_verticesId;
   
    Mesh* m_pMesh;
    Model m_model;
    Mesh* m_pGround;
    ModelMesh m_Cube;
    Model m_groundModel;
    
    LightShader* m_pShader;
    
    
    
    FBO* m_pFBO;
    std::vector<FBO*> m_pingpongFBOs;
    unsigned int quad_vertexBuffer_ID;
    
    Light m_light;
    Specular m_specular;
    std::map<eCameraType,Camera*> m_CameraMap;
    Camera* m_pCam;
    
    WindowEnv* m_pWindowEnv;
    
    

    
    Ctrl m_ctrl;
    SFXBloom* m_pSFXBloom;
    
    UIHandler m_UI;        
    
    eEvent2Process m_event2Process = eEVENTCAMERA;
    
    void createVBO(std::vector<unsigned int> verticesId);
    
    void createIBO(std::vector<unsigned int> verticesId);
    
    
    void CalcNormal(std::vector<unsigned int> verticesId, std::vector<Vertex>&  listVertex);
    
    
    
   
    void OpenGLRender();
    void OpenGLWorldRender();
    
    

    
    
    

};

#endif /* APPLICATION_H */

