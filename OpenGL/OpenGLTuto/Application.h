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



#include "LightShader.h"
#include "ZGL/FBO.h"

#include "ZGL/Texture.h"
#include "Model.h"


#include "glm.hpp"



#include <ctime>

#include "SFXBloom.h"

#include <ZGL/ZGLApp.h>



struct Ctrl
{
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




class Application  : public ZGLApp {
    
    friend class UIHandler;
    

   
    
public:
    Application();
    //Application(const Application& orig);
    virtual ~Application();
    
    virtual bool Init() override;
    
    
    
    
    void Destroy();
    
    
private:
     
    GLuint m_VBO;
    GLuint m_IBO;
    GLuint m_tex;
    
    
    
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
    
    

    
    Ctrl m_ctrl;
    SFXBloom* m_pSFXBloom;
    
       
    

    
    void createVBO(std::vector<unsigned int> verticesId);
    
    void createIBO(std::vector<unsigned int> verticesId);
    
    
    void CalcNormal(std::vector<unsigned int> verticesId, std::vector<Vertex>&  listVertex);
    
    
    
   
    virtual void OpenGLRender() override;
    virtual void ImguiDraw() override;
    
    void OpenGLWorldRender();
    
    
    std::string m_text = " texte a editer";
    clock_t m_timeCurrent;
    
    void CameraProjection();

    
    
    

};

#endif /* APPLICATION_H */

