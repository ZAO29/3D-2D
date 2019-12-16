/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   App.cpp
 * Author: nicolas
 * 
 * Created on December 15, 2019, 8:01 PM
 */

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






#include "App.h"

#include "ZGL/imgui/imgui.h"
#include "ZGL/imgui/imgui_impl_opengl3.h"
#include "ZGL/CameraFree.h"


#include <iostream>

#include <GL/glx.h>
#include <iostream>









bool App::Init()
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
        
        
        
        float val = 1.f;
	std::vector<ZVertex> cube_vertex_buffer_data = {
		ZVertex(glm::vec3(-val, -val, val),glm::vec2(0,0)), // Z = 1
		ZVertex(glm::vec3(val, -val,  val),glm::vec2(1,0)),
		ZVertex(glm::vec3(-val,  val, val),glm::vec2(0,1)),
		ZVertex(glm::vec3(-val,  val, val),glm::vec2(0,1)),
		ZVertex(glm::vec3(val, -val,  val),glm::vec2(1,0)),
		ZVertex(glm::vec3(val,  val,  val),glm::vec2(1,1)),
		ZVertex(glm::vec3(-val, -val, -val),glm::vec2(0,0)), // Z = -1
		ZVertex(glm::vec3(val, -val,  -val),glm::vec2(1,0)),
		ZVertex(glm::vec3(-val,  val, -val),glm::vec2(0,1)),
		ZVertex(glm::vec3(-val,  val, -val),glm::vec2(0,1)),
		ZVertex(glm::vec3(val, -val,  -val),glm::vec2(1,0)),
		ZVertex(glm::vec3(val,  val,  -val),glm::vec2(1,1)),
		ZVertex(glm::vec3(-val, val, val),glm::vec2(1,1)), // X = -1
		ZVertex(glm::vec3(-val,  val, -val),glm::vec2(1,0)),
		ZVertex(glm::vec3(-val,  -val,  val),glm::vec2(0,1)),
		ZVertex(glm::vec3(-val,  val,  -val),glm::vec2(1,0)),
		ZVertex(glm::vec3(-val,  -val,  val),glm::vec2(0,1)),
		ZVertex(glm::vec3(-val,  -val,  -val),glm::vec2(0,0)),	
		ZVertex(glm::vec3(val, -val, -val),glm::vec2(0,0)), // X = 1
		ZVertex(glm::vec3(val,  val, -val),glm::vec2(1,0)),
		ZVertex(glm::vec3(val, -val,  val),glm::vec2(0,1)),
		ZVertex(glm::vec3(val,  val, val),glm::vec2(1,1)),
		ZVertex(glm::vec3(val, val, -val),glm::vec2(1,0)),
		ZVertex(glm::vec3(val, -val, val),glm::vec2(0,1)),
		ZVertex(glm::vec3( -val, val,-val),glm::vec2(0,0)), // Y = 1
		ZVertex(glm::vec3(  val, val, -val),glm::vec2(1,0)),
		ZVertex(glm::vec3( -val, val,  val),glm::vec2(0,1)),
		ZVertex(glm::vec3(  val, val, val),glm::vec2(1,1)),
		ZVertex(glm::vec3(  val, val, -val),glm::vec2(1,0)),
		ZVertex(glm::vec3( -val, val,  val),glm::vec2(0,1)),
		ZVertex(glm::vec3(-val, -val,-val),glm::vec2(0,0)), // Y = -1
		ZVertex(glm::vec3(val,  -val, -val),glm::vec2(1,0)),
		ZVertex(glm::vec3(-val, -val,  val),glm::vec2(0,1)),
		ZVertex(glm::vec3(val,  -val,  val),glm::vec2(1,1)),
		ZVertex(glm::vec3(val,  -val, -val),glm::vec2(1,0)),
		ZVertex(glm::vec3(-val, -val,  val),glm::vec2(0,1))


	};
        
        m_cubeVBO.Init(cube_vertex_buffer_data);
            // les 2 FBO qui se renvoit la balle pour faire les traitement successifs
    m_pingpongFBOs.resize(2);
    for (auto& pFBO : m_pingpongFBOs)
    {
        pFBO = new FBO();
        pFBO->Init(m_pWindowEnv->getWindowWidth(),m_pWindowEnv->getWindowHeight(),1);
    }
    
    m_pTexBlack = new Texture(0.,0.,0.);
    
    m_pShader =  new BasicShader();
    
    m_pShader->Init();
    
    m_pQuadShader = new QuadShader();
    
    m_pQuadShader->Init();
    
    return true;
        
}
 
void App::OpenGLRender()
{
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_pingpongFBOs[0]->BindForWriting();
    m_pQuadShader->Enable();
    m_pTexBlack->Bind(GL_TEXTURE0);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    m_quadVBO.Render(GL_TRIANGLES);

    
    bool idFBO = true;
    

    glm::mat4 MVP = m_pCam->getProjectionView() *m_pCam->getView();
    m_pShader->Enable();
    m_pShader->SetMVP(MVP);

		int niter = 13;
		for (int i = 0;i <= niter;i++)
		{
			if (i == niter)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else
			{
				m_pingpongFBOs[idFBO]->BindForWriting();
			}
			
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_pShader->Enable();
			m_pingpongFBOs[!idFBO]->BindForReading(GL_TEXTURE0);

			glEnable(GL_DEPTH_TEST);
			m_cubeVBO.Render(GL_TRIANGLES);
			idFBO = !(idFBO);




			m_pingpongFBOs[idFBO]->BindForWriting();
			
			glClearColor(1.0f,1.0f,1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                        glDisable(GL_DEPTH_TEST);
                        m_pQuadShader->Enable();
			m_pingpongFBOs[!idFBO]->BindForReading(GL_TEXTURE0);
			m_quadVBO.Render(GL_TRIANGLES);

			idFBO = !(idFBO);

		}
}          




void App::OpenGLWorldRender()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    
    /*if(m_ctrl.m_rotate) m_model.Rotate(0.01,glm::vec3(0.,1.,0.));
    
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
    m_Cube.m_pMesh->Render(false);*/
}


void App::ImguiDraw()
{
     /*   static float f = 0.0f;
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
        
        
        
        ImGui::End();*/
}





/*
void App::Destroy()
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
    
    
    
}*/

void App::Destroy()
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


    
    
    


    
    for (auto& fbo : m_pingpongFBOs)
    {
        delete fbo; 
    }
}