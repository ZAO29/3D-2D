/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Application.cpp
 * Author: nicolas
 * 
 * Created on September 18, 2019, 3:08 PM
 */


#define GLM_FORCE_RADIANS

#include <iostream>
#include <assert.h> 
#include <vector>


//glm
#include <gtx/transform.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtc/constants.hpp>






#include "Application.h"
#include "Shaders.h"
#include "Texture.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glx.h"
#include "CameraFree.h"


// a inclure en dernier
#include <GL/glx.h>
#include <iostream>

void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, void* data)
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

void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data)
{    
    GLErrorCallback(source, type, id, severity, length, msg, data);
}

Application::Application():m_state(NOT_INIT),m_nbVertices(0),m_pWindowEnv(nullptr) {
}



Application::~Application() {
}




void Application::CalcNormal(std::vector<unsigned int> const verticesId, std::vector<Vertex>&  listVertex)
{
    std::cout<<__FILE__<<":"<<__FUNCTION__<<" NB TRI : "<< verticesId.size() << std::endl;
    
    glm::vec3 center = glm::vec3(0.);
    for (auto& v : listVertex) {
        center += v.m_pos;
    }
    
    center /= float(listVertex.size());
    
    
    
    
    for (unsigned int i = 0 ; i < verticesId.size() ; i += 3) {
        unsigned int Index0 = verticesId[i];
        unsigned int Index1 = verticesId[i + 1];
        unsigned int Index2 = verticesId[i + 2];
        glm::vec3 v1 = listVertex[Index1].m_pos - listVertex[Index0].m_pos;
        glm::vec3 v2 = listVertex[Index2].m_pos - listVertex[Index0].m_pos;
        glm::vec3 Normal = glm::normalize(glm::cross(v1,v2));
        
        
        // mettre les normal vers l'exterieur
        glm::vec3 insideDir = center - listVertex[Index1].m_pos;
        
        if( glm::dot(insideDir,Normal) > 0)
        {
            std::cout<<"Normal direction inside"<<std::endl;
            Normal *= -1.0f;
        }

        listVertex[Index0].m_normal += Normal;
        listVertex[Index1].m_normal += Normal;
        listVertex[Index2].m_normal += Normal;
    }
    std::cout<<__FILE__<<":"<<__FUNCTION__<<" entre "<<std::endl;
    for (auto& v : listVertex) {
        v.m_normal = glm::normalize(v.m_normal);
    }
}


void Application::createVBO(std::vector<unsigned int> verticesId)
{
    // std::vector<NBVector3D> vertexList = Parser3DModel::OBJParser("//home//nicolas//Documents//NBellot//cube.obj");
    
    m_vertices.resize(5);
    
    m_vertices[0] = Vertex(glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec2(0.,0.));
    m_vertices[1] = Vertex(glm::vec3(-0.5f, 0.0, 0.5f), glm::vec2(0.,1.));
    m_vertices[2] = Vertex(glm::vec3(0.5, 0.0, 0.5),  glm::vec2(1.,1.));
    m_vertices[3] = Vertex(glm::vec3(0.5, 0.0f, 0.0), glm::vec2(1.,0.));
    m_vertices[4] = Vertex(glm::vec3(0.0, 1.0f, 0.0), glm::vec2(0.5,0.5));
    

    CalcNormal(verticesId,m_vertices);
    
    
    
    
    // VBO : Vertex Buffer Object
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
   
}
    

void Application::createIBO(std::vector<unsigned int> verticesId)
{  
                           
    //m_nbVertices = 12;
    m_nbVertices = verticesId.size()*3;
    
    // IBO : Indice Buffer Object
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*verticesId.size(), &verticesId[0], GL_STATIC_DRAW);
}




bool Application::Init()
{
    m_pWindowEnv = WindowEnv::createWindowEnv(1280,720);
    
    m_pWindowEnv->init();
    //Window win = m_WindowEnv->getWindow();
    //bool init = ImGui_ImplGLX_Init(&win, m_WindowEnv->getDisplay() , false, 0);
    //ImGui_ImplGLX_NewFrame();
    
    // MESSAGE ERREUR OPENGL
    glDebugMessageCallback(GLErrorCallback,NULL);
    
    
    //MVP initialisation :
    
    
     confProjPerspective projConf;
    
    
    /*projConf.m_FOV=45.f;
    projConf.m_ratio = 4.0f / 3.0f;
    projConf.m_nearPlane = 0.1;
    projConf.m_farPlane = 100.0;*/
    
    //m_projectionView.setProjectionConfig(projConf);
    
    
    /*************** FBO ***********/
    // ce FBO rend dans 2 textures 
    m_pFBO = new FBO();
    m_pFBO->Init(m_pWindowEnv->getWindowWidth(),m_pWindowEnv->getWindowHeight(),2);
    
    
    // les 2 FBO qui se renvoit la balle pour faire les traitement successifs
    m_pingpongFBOs.resize(2);
    for (auto& pFBO : m_pingpongFBOs)
    {
        pFBO = new FBO();
        pFBO->Init(m_pWindowEnv->getWindowWidth(),m_pWindowEnv->getWindowHeight(),1);
    }
     
    
     
    m_CameraMap[TRACKBALLCAMERA]= new CameraTrackBall();
    m_CameraMap[FREECAMERA]= new CameraFree();
    m_pCam = m_CameraMap[FREECAMERA];
    

    
    
    m_pShader = new LightShader();
    
    m_pShader->Init();
    
    m_pShader->Enable();
    
    m_pSFXBloom = new SFXBloom();
    
    m_pSFXBloom->Init();
    

    
    
    //0 car la TEXTURE binder dans GL_TEXTURE0
    m_light.Color = glm::vec3(1.0,1.0,1.0);
    m_light.AmbientIntensity = 0.5;
    m_light.DiffuseIntensity = 0.5;
    m_light.DirectionOrPos = glm::normalize(glm::vec3(1.0,0.0,10.0));
    
    m_state = Application::INITIALIZED;

    glEnable(GL_DEPTH_TEST);
         
     m_pMesh = new Mesh();
     m_pGround = new Mesh();
     m_Cube.m_pMesh = new Mesh();
       
    //std::string filename3DModel = "//home//nicolas//Documents//Model3D//Voiture//Audi R8//Models//Audi R8.fbx";
    //std::string texFolder = "//home//nicolas//Documents//Model3D//Voiture//Audi R8//Textures//";
   
     
     // AVION
     //std::string filename3DModel = "//home//nicolas//Documents//Model3D//PiperPa34//31-pipersenecav_fbx//PiperSenecaV_ldgDown.FBX";
     //std::string texFolder = "//home//nicolas//Documents//Model3D//PiperPa34//49-pipersenecav_tex//";

     
     
     // SCENEGRAPH 1
     //std::string filename3DModel = "//home//nicolas//Documents//Model3D//scenegraph//box.fbx";
     //std::string filename3DModel = "//home//nicolas//Documents//Model3D//scenegraph//cross.fbx";
     //std::string texFolder = "";
     
     //ICOSPHERE 
     //std::string filename3DModel = "//home//nicolas//Documents//Model3D//scenegraph//icosphere.fbx";
     //std::string texFolder = "";
     
     
     //std::string filename3DModel = "//home//nicolas//Documents//Model3D//scenegraph//woodbox_normal.fbx";
     //std::string texFolder = "//home//nicolas//Documents//Model3D//NormalMapModel//";
     //std::string texFolder = "";
     
     // OLDMAN
     std::string filename3DModel = "//home//nicolas//Documents//Model3D//OldMan//muro.fbx";
     std::string texFolder = "";
     
     
    ///// IMGUI ////// 
     UIHandler::Init(m_pWindowEnv);
    //std::string filenameGround = "//home//nicolas//Downloads//ogldev-source//Content//quad.obj";
     //std::string filenameGround = "//home//nicolas/Documents//Model3D//ground//damier.fbx";
    
     
     
   
    m_pMesh->LoadMesh(filename3DModel,texFolder);
    
    m_Cube.m_pMesh->LoadMesh("//home//nicolas//Documents//Model3D//cube_white.fbx");
    
    
    std::string filenameGround = "//home//nicolas//Documents//Model3D//ground//polymountains.fbx";
    m_pGround->m_importConfig.m_minSize = 60.0;
    m_pGround->m_importConfig.m_maxSize = 100.0;
    m_pGround->LoadMesh(filenameGround);
    m_groundModel.Translate(glm::vec3(0.,-3.,0.));
}
 
bool Application::Run()
{

    m_state = RUNNING;
    processEvents();
    
    glClearColor( 0, 0., 0., 1 );
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    OpenGLRender();
    if(m_ctrl.m_imguiCtrl)
        m_UI.Render(*this);
 
    m_pWindowEnv->swapBuffer();
    
    return (m_state == RUNNING);
}

void Application::OpenGLRender()
{

    glEnable(GL_DEPTH_TEST);
    m_pShader->Enable();
    if(!m_ctrl.m_EnableBloomSFX)
    {
       glBindFramebuffer(GL_FRAMEBUFFER, 0); // rebind vers l'ecran
       OpenGLWorldRender();
       return;
    }else
    {
            m_pFBO->BindForWriting();  // pour ecrire dans les deux textures  
            OpenGLWorldRender();
    }
    
    m_pSFXBloom->Render(m_pFBO,m_pingpongFBOs);
    
    
    /*
    m_pFBO->BindForReading(GL_TEXTURE0,1);
    

    // BLURR MULTIPLE PASS
    m_pBlurShader->Enable();
    
    bool horizontal = true;
    for (unsigned int i=0;i< m_SFXBloom.m_nbPass*2;i++)
    {
        horizontal= !(horizontal);
        m_pBlurShader->SetHorizontal(horizontal);
        
        m_pingpongFBOs[horizontal]->BindForWriting();
        m_pingpongFBOs[horizontal]->RenderQuad();
        m_pingpongFBOs[horizontal]->BindForReading(GL_TEXTURE0);
        
        
    }
    
    //BLENDING 
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // rebind vers l'ecran
    m_pBlendingShader->Enable();
    m_pBlendingShader->SetBlurrTextureUnit(0);
    m_pBlendingShader->SetRenderTextureUnit(1);
    m_pFBO->BindForReading(GL_TEXTURE1,0); // rendu de la scene
    if(m_SFXBloom.m_nbPass > 0)
        m_pingpongFBOs[horizontal]->BindForReading(GL_TEXTURE0); // blurred light
    else
        m_pBlackTex->Bind(GL_TEXTURE0);
    m_pFBO->RenderQuad();*/
    
    
    
    
    
    
    

}




void Application::OpenGLWorldRender()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    
    if(m_ctrl.m_rotate) m_model.Rotate(0.01,glm::vec3(0.,1.,0.));
    
    m_MVP = m_pCam->getProjectionView() *m_pCam->getView()* m_model.getMat();
    m_pShader->SetMVP(m_MVP);
    m_pShader->SetWorldMat(m_model.getMat());
    m_pShader->SetLight(m_light);
    m_pShader->SetWorldEye(m_pCam->getEyePos());
    m_pShader->SetDebug(false);
    m_pShader->SetTresholdBrightness(m_pSFXBloom->m_config.m_tresholdBrightNess);
    m_pShader->SetColorTextureUnit(COLOR_UNIT);
    m_pShader->SetNormalMapTextureUnit(NORMALMAP_UNIT);
    
    m_pShader->SetSpecular(m_specular);
    m_pMesh->Render(m_ctrl.m_useNormalMap);
    
    // GROUND
    m_MVP = m_pCam->getProjectionView() *m_pCam->getView()* m_groundModel.getMat();
    m_pShader->SetMVP(m_MVP);
    m_pShader->SetWorldMat(m_groundModel.getMat());
    m_pGround->Render(m_ctrl.m_useNormalMap);
    
    //CUBE pour le DEBUG
    if(!m_ctrl.m_debugCube)
        return; 
    
    glm::vec3 posCube = m_light.DirectionOrPos;
    if (m_light.isDir)
    {
        posCube = 50.0f * glm::normalize(posCube);
    }    
    m_Cube.m_model.SetPosition(posCube);
    glm::mat4 cubeMat = m_Cube.m_model.getMat();
    m_MVP = m_pCam->getProjectionView() *m_pCam->getView()* cubeMat;
    m_pShader->SetMVP(m_MVP);
    m_pShader->SetWorldMat(cubeMat);
    m_pShader->SetDebug(true);
    m_Cube.m_pMesh->Render(false);
}



void Application::Destroy()
{
    m_pWindowEnv->destroy();
    
    WindowEnv::deleteWindowEnv();
    m_pWindowEnv = NULL;
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    m_state = CLOSE;
    
    // fichier qui configure la taille des fenetres
    std::remove( "//home//nicolas//Repos//trunk//src//VisualSystem//AssetEditor//imgui.ini" );
    

    for (auto it = m_CameraMap.begin(); it != m_CameraMap.end();)
    {
        it = m_CameraMap.erase(it);
    }
    
    m_pCam = NULL;

    delete m_pMesh;
    delete m_pGround;
    delete m_Cube.m_pMesh;
    
    
    
    m_Cube.m_pMesh = NULL;
    m_pMesh = NULL;
    m_pGround = NULL;
    
    delete m_pSFXBloom;
    
    
    delete m_pFBO;
    
    for (auto& fbo : m_pingpongFBOs)
    {
        delete fbo; 
    }
    
    
    
}

void Application::processEvents()
{
    while (XPending(m_pWindowEnv->getDisplay()) > 0) 
    {
        int32_t code;

        XEvent event;
        XNextEvent(m_pWindowEnv->getDisplay(), &event); 
            
        if (!m_ctrl.m_imguiCtrl)
        {
            switch (m_event2Process)
            {
                case eEVENTCAMERA:
                    m_pCam->OnEvent(event);
                    break;
                case eEVENTLIGHT:
                    OnLightEvent(event);
                    break;
                case eEVENTMODEL:
                    OnModelEvent(event);
            }
        }else
        {
            UIEvents(event);
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

                    case XK_c:
                        if(m_event2Process != eEVENTCAMERA)
                        {
                            m_event2Process = eEVENTCAMERA;
                            std::cout<<"CONTROL CAM"<<std::endl;
                        }
                        
                        
                        break;
                    case XK_l:
                        if(m_event2Process != eEVENTLIGHT)
                        {
                            m_event2Process = eEVENTLIGHT;
                            std::cout<<"CONTROL LIGHT"<<std::endl;
                            
                        }
                        
                        break; 
                    case XK_m :
                        if(m_event2Process != eEVENTMODEL)
                        {
                            m_event2Process = eEVENTMODEL;
                            std::cout<<"CONTROL MODEL"<<std::endl;
                            
                        }
                        break;
                        
                    case XK_Escape:
                        m_state = TOCLOSE;
                        std::cout<<" Escape : "<< code <<std::endl;
                        return;

                    case XK_F1:
                        m_ctrl.m_imguiCtrl = !(m_ctrl.m_imguiCtrl);
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

void Application::UIEvents(XEvent event)
{
   ImGui_ImplGlx_OnEvent(&event);
}




void Application::OnModelEvent(XEvent event)
{
        if(event.type != KeyPress)
        return;

    int32_t keycode = XLookupKeysym(&event.xkey, 0);
    
    float threshold = 10e-5;

    switch (keycode) 
    {

        case XK_Up:
            
            m_model.Scale(1.1f);
            //m_eye += m_speed*m_up;
            //m_deltaDegP += m_speed;
            break;
        case XK_Down:
            m_model.Scale(float(1.f/1.1f));
           
            break;
        case XK_Right:
            if(m_specular.intensity < threshold)
            {
                m_specular.intensity = threshold;
            }
            
            m_specular.intensity*=1.1;
            std::cout<<"specular intensity "<< m_specular.intensity <<std::endl;
            break;
        case XK_Left:
            m_specular.intensity/=1.1;
            std::cout<<"specular intensity "<< m_specular.intensity <<std::endl;
            if(m_specular.intensity < threshold)
            {
                m_specular.intensity = 0.;
            }
            break;
        case XK_w:
            m_specular.power*=1.1;
            
            std::cout<<"specular power "<< m_specular.power <<std::endl;
            break;
        case XK_x:
            m_specular.power/=1.1;
            std::cout<<"specular power "<< m_specular.power <<std::endl;
            break;
        case XK_n:
            m_ctrl.m_useNormalMap=true;
            std::cout<<"normalMap "<< m_ctrl.m_useNormalMap <<std::endl;
            break; 
        case XK_b:
            m_ctrl.m_useNormalMap=false;
            std::cout<<"normalMap "<< m_ctrl.m_useNormalMap <<std::endl;
            break; 
        
    }
}


void Application::OnLightEvent(XEvent event)
{
    if(event.type != KeyPress)
        return;

    int32_t keycode = XLookupKeysym(&event.xkey, 0);
    
    

    switch (keycode) 
    {

        case XK_Up:
            
            m_light.AmbientIntensity*=1.1;
            std::cout<< "Ambient Intensity" << m_light.AmbientIntensity << std::endl;
            //m_eye += m_speed*m_up;
            //m_deltaDegP += m_speed;
            break;
        case XK_Down:
            m_light.AmbientIntensity/=1.1;
            std::cout<< "Ambient Intensity" << m_light.AmbientIntensity << std::endl;
           
            break;
            
        case XK_Right:
            
            m_light.DiffuseIntensity*=1.1;
            std::cout<< "Diffuse Intensity" << m_light.DiffuseIntensity << std::endl;
            //m_eye += m_speed*m_up;
            //m_deltaDegP += m_speed;
            break;
        case XK_Left:
            m_light.DiffuseIntensity/=1.1;
            std::cout<< "Diffuse Intensity" << m_light.DiffuseIntensity << std::endl;
           
            break;

    }
}
