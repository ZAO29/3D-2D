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
#include "ZGL/Texture.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glx.h"
#include "ZGL/CameraFree.h"


// a inclure en dernier
#include <GL/glx.h>
#include <iostream>






Application::Application():m_nbVertices(0){
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
    ZGLApp::Init();
    
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
     //std::string filename3DModel = "//home//nicolas//Documents//Model3D//scen/home/nicolas/Documents/3D_modelgraph//icosphere.fbx";
     //std::string texFolder = "";
     
     
     //std::string filename3DModel = "//home//nicolas//Documents//Model3D//scenegraph//woodbox_normal.fbx";
     //std::string texFolder = "//home//nicolas//Documents//Model3D//NormalMapModel//";
     //std::string texFolder = "";
     
     // OLDMAN
     std::string filename3DModel = "/home/nicolas/Documents/3D_model/muro.fbx";
     std::string texFolder = "/home/nicolas/Documents/3D_model/";
     
     
    ///// IMGUI ////// 
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


void Application::ImguiDraw()
{
        static float f = 0.0f;
          unsigned int windowFlag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;

        ImGui::Begin("Configure",NULL,windowFlag);                          // Create a window called "Hello, world!" and append into it.


        ImGui::Checkbox("Use Normal map", &m_ctrl.m_useNormalMap);
        ImGui::Checkbox("Rotate model", &m_ctrl.m_rotate);         
        ImGui::Checkbox(" Debug Cube",&m_ctrl.m_debugCube);
        
        clock_t current = clock();
        float elapsedTime = (current - m_timeCurrent) / float(CLOCKS_PER_SEC)*1000.0f;
        
        
        ImGui::Text(" Render time (ms) : %f",elapsedTime);
        m_timeCurrent = current;
        
        // exemple de texte
        char  ctext[256]; 
        strcpy(ctext, m_text.c_str());
        if(ImGui::InputText(" input text", ctext, 256))
        {
            m_text = ctext;
        }
        
        
        if(ImGui::TreeNode(" Light"))
        {
            //ImGui::Begin("Light ",NULL,windowFlag);
                ImGui::SliderFloat("Diffuse intensity", &m_light.DiffuseIntensity, 0.0f, 10.0f);
                ImGui::SliderFloat("Ambient intensity", &m_light.AmbientIntensity, 0.0f, 10.0f);
                ImGui::Text(" Specular ");
                ImGui::SliderFloat("intensity", &m_specular.intensity, 0.0f, 10.0f);
                ImGui::SliderFloat("power", &m_specular.power, 0.0f, 10.0f);
                ImVec4 clear_color = ImVec4(m_light.Color.x, m_light.Color.y, m_light.Color.z, 1.00f);
                
            
                // Edit 1 float using a slider from 0.0f to 1.0f
                if(ImGui::ColorEdit3("Light color", (float*)&clear_color)) // Edit 3 floats representing a color
                {
                    m_light.Color = glm::vec3(clear_color.x,clear_color.y,clear_color.z);
                }
                
                ImGui::Checkbox(" SUN ",&m_light.isDir);
                
                if(m_light.isDir )
                {// DIRECTION
                    float Hdeg = getHeadingDeg(m_light.DirectionOrPos);
                    float Pdeg = getPitchDeg(m_light.DirectionOrPos);



                    //ImGui::SliderFloat("Heading (deg)", &Hdeg, 0.0f, 360.0f)
                    ImGui::Text(" Direction ");

                    bool HorPchanged = ImGui::SliderFloat("H (deg)", &Hdeg, 0.0f, 360.0f);
                         HorPchanged = ImGui::SliderFloat("P (deg)", &Pdeg, -90.0f, 90.0f) || HorPchanged;

                    if(HorPchanged)  
                    {
                        setPitchAndHeadingDeg(m_light.DirectionOrPos,Hdeg,Pdeg);
                    }

                    ImGui::Text(" direction : %f | %f | %f" , m_light.DirectionOrPos.x, m_light.DirectionOrPos.y, m_light.DirectionOrPos.z);
                }else
                {  // POSITIONNAL LIGHT
                   ImGui::Text(" Position "); 
                   ImGui::SliderFloat("X ", &m_light.DirectionOrPos.x, -100.0f, 100.0f);
                   ImGui::SliderFloat("Y ", &m_light.DirectionOrPos.y, -100.0f, 100.0f);
                   ImGui::SliderFloat("Z ", &m_light.DirectionOrPos.z, -100.0f, 100.0f);
                }
            ImGui::TreePop();    
        }
        
        
        if(ImGui::TreeNode(" Camera "))
        {
            float norm = m_pCam->getScaleEye();         
            if(ImGui::SliderFloat(" distance ",&norm,0.1f,10.0f))
                m_pCam->setScaleEye(norm);
            
            
            
            /* gestion changement du type de camera */
            bool isFreeCamera = (m_pCam == m_CameraMap[FREECAMERA]);
            bool isTrackBallCamera = (m_pCam == m_CameraMap[TRACKBALLCAMERA]);
            bool newIsFreeCamera = isFreeCamera;
            bool newIsTrackBallCamera = isTrackBallCamera;
            
            ImGui::Checkbox(" TRACKBALL ",&newIsTrackBallCamera);
            ImGui::Checkbox(" FREE ",&newIsFreeCamera);
            
            bool becomeTrackBallCam = (newIsTrackBallCamera && !isTrackBallCamera);
            
            bool cameraTypeChanged = (newIsFreeCamera && !isFreeCamera) || becomeTrackBallCam;
                                     ;
            // la camera a changée de type
            if(cameraTypeChanged)
            {
               eCameraType newCamType =  FREECAMERA;
               
               if(becomeTrackBallCam)
                   newCamType = TRACKBALLCAMERA;
               
               
                Camera* m_pOldCam = m_pCam;
                m_pCam = m_CameraMap[newCamType];
                
                m_pCam->setEyePos(m_pOldCam->getEyePos());
                m_pCam->setDirection(m_pOldCam->getDirection());
                m_pCam->setUp(m_pOldCam->getUp());
                m_pCam->setRight(m_pOldCam->getRight());
            }
            
            CameraProjection();                         
                                     
            ImGui::TreePop();    
        }
        
        if(ImGui::TreeNode(" Bloom SFX "))
        {
            ImGui::Checkbox(" Enable ",&m_ctrl.m_EnableBloomSFX);
            ImGui::SliderFloat("BrightNess threshold", &m_pSFXBloom->m_config.m_tresholdBrightNess, 0.0f, 10.0f);
            int nbPass = m_pSFXBloom->m_config.m_nbPass;
            if(ImGui::SliderInt("number of Blur Pass", &nbPass, 0, 100))
                m_pSFXBloom->m_config.m_nbPass = nbPass;
            ImGui::TreePop();
        }
        
        
        
        ImGui::End();
}



void Application::CameraProjection()
{
    if(ImGui::TreeNode(" Projection "))
    {    
            bool isOrtho = (m_pCam->m_projectionType == Camera::ORTHO);
            bool isPerspective = (m_pCam->m_projectionType == Camera::PERSPECTIVE);
            bool newIsOrtho = isOrtho;
            bool newIsPerspective = isPerspective;
            
            
            
            ImGui::Checkbox(" ORTHO ",&newIsOrtho);
            ImGui::Checkbox(" PERSPECTIVE ",&newIsPerspective);
            
            
            if(newIsOrtho && !isOrtho)
                m_pCam->m_projectionType = Camera::ORTHO;
            if(newIsPerspective && !isPerspective)
                m_pCam->m_projectionType = Camera::PERSPECTIVE;
           
            
            
            
            if(isOrtho)
            {
                ImGui::SliderFloat("left", &m_pCam->m_projOrtho.m_left,-500.0f, 0.0f);
                ImGui::SliderFloat("right", &m_pCam->m_projOrtho.m_right, 0.0f, 500.0f);
                ImGui::SliderFloat("bottom", &m_pCam->m_projOrtho.m_bottom,-500.0f, 0.0f);
                ImGui::SliderFloat("top", &m_pCam->m_projOrtho.m_top,0.0f, 500.0f);
                ImGui::SliderFloat("near plane", &m_pCam->m_projOrtho.m_nearPlane, 0.0f, 100.0f);
                ImGui::SliderFloat("far plane", &m_pCam->m_projOrtho.m_farPlane, 100.0f, 10000.0f);
            }else
            {
                ImGui::SliderFloat("verticalFOV", &m_pCam->m_projPerspective.m_degVerticalFOV,0.0f, 90.0f);
                ImGui::SliderFloat("ratio width/height", &m_pCam->m_projPerspective.m_ratio, 0.0f, 500.0f);
                ImGui::SliderFloat("near Plane", &m_pCam->m_projOrtho.m_bottom,-500.0f, 0.0f);
                ImGui::SliderFloat("far Plane", &m_pCam->m_projOrtho.m_top,-500.0f, 0.0f);
            }
                 
            ImGui::TreePop();
    }
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
