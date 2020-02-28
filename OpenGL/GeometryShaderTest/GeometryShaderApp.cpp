/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeometryShaderApp.cpp
 * Author: nicolas
 * 
 * Created on February 27, 2020, 8:47 PM
 */

#include "GeometryShaderApp.h"

#define GLM_FORCE_RADIANS

#include <iostream>
#include <assert.h> 
#include <vector>


//glm
#include <gtx/transform.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtc/constants.hpp>








#include "ZGL/imgui/imgui.h"
#include "ZGL/imgui/imgui_impl_opengl3.h"
#include "ZGL/CameraFree.h"


#include <iostream>

#include <GL/glx.h>
#include <iostream>









bool App::Init()
{
    if(!ZGLApp::Init())
        return false;
    
   
    
    
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
        
        
        
    Shader::setShaderPath("../GeometryShaderTest");  
    return m_shader.Init("shader",true);   
     
}
 
void App::OpenGLRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_shader.Enable();
    glLineWidth(5);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    m_quadVBO.Render(GL_POINTS);
  
}          

void App::ImguiDraw()
{

}






void App::Destroy()
{
    m_pWindowEnv->destroy();
    
    WindowEnv::deleteWindowEnv();
    m_pWindowEnv = NULL;
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
 
    

    
    
    
}

