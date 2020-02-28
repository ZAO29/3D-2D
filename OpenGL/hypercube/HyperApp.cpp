/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HyperApp.cpp
 * Author: nicolas
 * 
 * Created on December 17, 2019, 8:25 PM
 */

#include "HyperApp.h"



bool HyperApp::Init()
{
        ZGLApp::Init();
    
   
    
    
     float eps = 0.05;
	std::vector<ZVertex> quad_vertex_buffer_data = { 
		ZVertex(glm::vec3(-1.0f+eps, -1.0f+eps, 0.0f),glm::vec2(0,0)),
		ZVertex(glm::vec3(1.0f-eps, -1.0f+eps, 0.0f),glm::vec2(1,0)),
		ZVertex(glm::vec3(-1.0f+eps,  1.0f-eps, 0.0f),glm::vec2(0,1)),
		ZVertex(glm::vec3(-1.0f+eps,  1.0f-eps, 0.0f),glm::vec2(0,1)),
		ZVertex(glm::vec3(1.0f-eps, -1.0f+eps, 0.0f),glm::vec2(1,0)),
		ZVertex(glm::vec3(1.0f-eps,  1.0f-eps, 0.0f),glm::vec2(1,1))
	};


	m_quadVBO.Init(quad_vertex_buffer_data);
        
        m_pTexBlack = new Texture(0.,0.,0.);
        
        m_pQuadShader = new QuadShader();
        m_pQuadShader->Init("../ZGL/");
        
        m_pHyperCubeShader = new BasicShader();
        m_pHyperCubeShader->Init();
        

        
      
        m_FBO.Init(m_pWindowEnv->getWindowWidth(),m_pWindowEnv->getWindowHeight(),1);
        
        
        
        std::vector<ZVertex4> vertices;
        float a = 2;
        // 1 - a -a .. ..
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,-a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,-a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,a),glm::vec2(0,1)));
        
        
        // 2- a .. a ..
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,-a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,-a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,a),glm::vec2(0,1)));
        
         // 3 - a .. .. a
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,a),glm::vec2(0,1)));
        
        
        // 4 - .. a .. a
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,-a),glm::vec2(0,1)));
        
        // 5 .. .. a a
         vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,-a),glm::vec2(0,1)));
        
        // 6 .. a a ..
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,-a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,-a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,-a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,-a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,-a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(a,-a,-a,a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,-a,a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(a,a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(a,-a,a,a),glm::vec2(0,1)));
        
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,-a,a),glm::vec2(0,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,-a,a),glm::vec2(1,0)));
        vertices.push_back(ZVertex4(glm::vec4(-a,a,a,a),glm::vec2(1,1)));
        vertices.push_back(ZVertex4(glm::vec4(-a,-a,a,a),glm::vec2(0,1)));
        
        m_hypercubeVBO.Init(vertices);
        
        
        

}

void HyperApp::OpenGLRender()
{
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_FBO.BindForWriting();
    m_pQuadShader->Enable();
    m_pTexBlack->Bind(GL_TEXTURE0);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_quadVBO.Render(GL_TRIANGLES);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_pHyperCubeShader->Enable();
    glm::mat4 MVP = m_pCam->getProjectionView() *m_pCam->getView();
    m_pHyperCubeShader->SetMVP(MVP);
    m_FBO.BindForReading(GL_TEXTURE0);
    m_hypercubeVBO.Render(GL_QUADS);
    

    
}
