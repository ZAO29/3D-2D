/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZGLApp.cpp
 * Author: nicolas
 * 
 * Created on December 5, 2019, 9:23 PM
 */

#include "ZGLApp.h"
#include "CameraFree.h"
#include "CameraTrackBall.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glx.h"

void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data)
{    
        std::string sType;
    switch(type)
    {
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        sType = "DEPRECATED";
        break;
    case GL_DEBUG_TYPE_ERROR:
        sType = "ERROR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        sType = "UNDEFINED";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        sType = "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        sType = "PORTABILITY";
        break;
    default:
        return;
        sType = "OTHER";
        break;
    }
    std::cout << "GL_DEBUG "<<sType.c_str()<<" : "<<msg<<std::endl;
}



ZGLApp::ZGLApp()
{
}

ZGLApp::ZGLApp(const ZGLApp& orig)
{
}

ZGLApp::~ZGLApp()
{
}


bool ZGLApp::Run()
{
        m_state = RUNNING;
    processEvents();
    
    glClearColor( 0, 0., 0., 1 );
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    OpenGLRender();
    if(m_bImguiCtrl)
        ImguiRender();
 
    m_pWindowEnv->swapBuffer();
    
    return (m_state == RUNNING);
}


bool ZGLApp::Init()
{
    m_pWindowEnv = WindowEnv::createWindowEnv(1280,720);
    
    m_pWindowEnv->init();

    
    // MESSAGE ERREUR OPENGL
    glDebugMessageCallback(GLErrorCallback,NULL);
    
    
    m_CameraMap[TRACKBALLCAMERA]= new CameraTrackBall();
    m_CameraMap[FREECAMERA]= new CameraFree();
    m_pCam = m_CameraMap[FREECAMERA];
    
    InitImgui();
    m_state = INITIALIZED;
    return true;
            
}


void ZGLApp::InitImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    
    io.DisplaySize = ImVec2(m_pWindowEnv->getWindowWidth(),m_pWindowEnv->getWindowHeight());
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    Window win = m_pWindowEnv->getWindow();
    
    ImGui_ImplGlx_Init(&win, true);
    
    
    
    
    // Setup Platform/Renderer bindings
    //ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 460";
    ImGui_ImplOpenGL3_Init(glsl_version);
}


void ZGLApp::processEvents()
{
    while (XPending(m_pWindowEnv->getDisplay()) > 0) 
    {
        int32_t code;

        XEvent event;
        XNextEvent(m_pWindowEnv->getDisplay(), &event); 
            
        if (!m_bImguiCtrl)
        {
                    m_pCam->OnEvent(event);
        }else
        {
            ImGui_ImplGlx_OnEvent(&event);
        }
        
        
        
        switch (event.type) 
        {
            case Expose:
                break;

            case ConfigureNotify:
                m_pWindowEnv->setWindowSize(event.xconfigure.width, event.xconfigure.height);
                break;

            case MotionNotify:    
                break;

            case ButtonPress:
                break;

            case KeyPress:
                code = XLookupKeysym(&event.xkey, 0);
                switch (code) 
                {                        
                    case XK_Escape:
                        m_state = TOCLOSE;
                        std::cout<<" Escape : "<< code <<std::endl;
                        return;

                    case XK_a:
                        m_bImguiCtrl = !(m_bImguiCtrl);
                        break;
                }
               
                break;
            

            case KeyRelease:                
                break;

        case ClientMessage:
            break;
        }


    }
}


void ZGLApp::ImguiRender()
{
    ImGui::NewFrame();
    
    ImguiDraw();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
