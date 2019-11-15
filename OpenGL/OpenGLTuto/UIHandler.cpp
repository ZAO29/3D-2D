/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UIHandler.cpp
 * Author: nicolas
 * 
 * Created on October 8, 2019, 4:46 PM
 */

#include "UIHandler.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include"Application.h"
#include "imgui/imgui_impl_glx.h"





UIHandler::UIHandler() {
}

UIHandler::UIHandler(const UIHandler& orig) {
}

UIHandler::~UIHandler() {
}


void UIHandler::Render(Application & app)
{
    //ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    
    //ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
    bool show_demo_window = true;
    
    unsigned int windowFlag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
    
    

   


        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
       //if (show_demo_window)
       //    ImGui::ShowDemoWindow(&show_demo_window);


            static float f = 0.0f;


        ImGui::Begin("Configure",NULL,windowFlag);                          // Create a window called "Hello, world!" and append into it.


        ImGui::Checkbox("Use Normal map", &app.m_ctrl.m_useNormalMap);
        ImGui::Checkbox("Rotate model", &app.m_ctrl.m_rotate);         
        ImGui::Checkbox(" Debug Cube",&app.m_ctrl.m_debugCube);
        
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
                ImGui::SliderFloat("Diffuse intensity", &app.m_light.DiffuseIntensity, 0.0f, 10.0f);
                ImGui::SliderFloat("Ambient intensity", &app.m_light.AmbientIntensity, 0.0f, 10.0f);
                ImGui::Text(" Specular ");
                ImGui::SliderFloat("intensity", &app.m_specular.intensity, 0.0f, 10.0f);
                ImGui::SliderFloat("power", &app.m_specular.power, 0.0f, 10.0f);
                ImVec4 clear_color = ImVec4(app.m_light.Color.x, app.m_light.Color.y, app.m_light.Color.z, 1.00f);
                
            
                // Edit 1 float using a slider from 0.0f to 1.0f
                if(ImGui::ColorEdit3("Light color", (float*)&clear_color)) // Edit 3 floats representing a color
                {
                    app.m_light.Color = glm::vec3(clear_color.x,clear_color.y,clear_color.z);
                }
                
                ImGui::Checkbox(" SUN ",&app.m_light.isDir);
                
                if(app.m_light.isDir )
                {// DIRECTION
                    float Hdeg = getHeadingDeg(app.m_light.DirectionOrPos);
                    float Pdeg = getPitchDeg(app.m_light.DirectionOrPos);



                    //ImGui::SliderFloat("Heading (deg)", &Hdeg, 0.0f, 360.0f)
                    ImGui::Text(" Direction ");

                    bool HorPchanged = ImGui::SliderFloat("H (deg)", &Hdeg, 0.0f, 360.0f);
                         HorPchanged = ImGui::SliderFloat("P (deg)", &Pdeg, -90.0f, 90.0f) || HorPchanged;

                    if(HorPchanged)  
                    {
                        setPitchAndHeadingDeg(app.m_light.DirectionOrPos,Hdeg,Pdeg);
                    }

                    ImGui::Text(" direction : %f | %f | %f" , app.m_light.DirectionOrPos.x, app.m_light.DirectionOrPos.y, app.m_light.DirectionOrPos.z);
                }else
                {  // POSITIONNAL LIGHT
                   ImGui::Text(" Position "); 
                   ImGui::SliderFloat("X ", &app.m_light.DirectionOrPos.x, -100.0f, 100.0f);
                   ImGui::SliderFloat("Y ", &app.m_light.DirectionOrPos.y, -100.0f, 100.0f);
                   ImGui::SliderFloat("Z ", &app.m_light.DirectionOrPos.z, -100.0f, 100.0f);
                }
            ImGui::TreePop();    
        }
        
        
        if(ImGui::TreeNode(" Camera "))
        {
            float norm = app.m_pCam->getScaleEye();         
            if(ImGui::SliderFloat(" distance ",&norm,0.1f,10.0f))
                app.m_pCam->setScaleEye(norm);
            
            
            
            /* gestion changement du type de camera */
            bool isFreeCamera = (app.m_pCam == app.m_CameraMap[FREECAMERA]);
            bool isTrackBallCamera = (app.m_pCam == app.m_CameraMap[TRACKBALLCAMERA]);
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
               
               
                Camera* m_pOldCam = app.m_pCam;
                app.m_pCam = app.m_CameraMap[newCamType];
                
                app.m_pCam->setEyePos(m_pOldCam->getEyePos());
                app.m_pCam->setDirection(m_pOldCam->getDirection());
                app.m_pCam->setUp(m_pOldCam->getUp());
                app.m_pCam->setRight(m_pOldCam->getRight());
            }
            
            CameraProjection(app);                         
                                     
            ImGui::TreePop();    
        }
        
        if(ImGui::TreeNode(" Bloom SFX "))
        {
            ImGui::Checkbox(" Enable ",&app.m_ctrl.m_EnableBloomSFX);
            ImGui::SliderFloat("BrightNess threshold", &app.m_pSFXBloom->m_config.m_tresholdBrightNess, 0.0f, 10.0f);
            int nbPass = app.m_pSFXBloom->m_config.m_nbPass;
            if(ImGui::SliderInt("number of Blur Pass", &nbPass, 0, 100))
                app.m_pSFXBloom->m_config.m_nbPass = nbPass;
            ImGui::TreePop();
        }
        
        
        
        ImGui::End();
            
            



       

    // Rendering
    ImGui::Render();
    
    
    
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



void UIHandler::Init(WindowEnv* pwinEnv)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    
    io.DisplaySize = ImVec2(pwinEnv->getWindowWidth(),pwinEnv->getWindowHeight());
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    Window win = pwinEnv->getWindow();
    
    ImGui_ImplGlx_Init(&win, true);
    
    
    
    
    // Setup Platform/Renderer bindings
    //ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 460";
    ImGui_ImplOpenGL3_Init(glsl_version);
}


void UIHandler::CameraProjection(Application & app)
{
    if(ImGui::TreeNode(" Projection "))
    {    
            bool isOrtho = (app.m_pCam->m_projectionType == Camera::ORTHO);
            bool isPerspective = (app.m_pCam->m_projectionType == Camera::PERSPECTIVE);
            bool newIsOrtho = isOrtho;
            bool newIsPerspective = isPerspective;
            
            
            
            ImGui::Checkbox(" ORTHO ",&newIsOrtho);
            ImGui::Checkbox(" PERSPECTIVE ",&newIsPerspective);
            
            
            if(newIsOrtho && !isOrtho)
                app.m_pCam->m_projectionType = Camera::ORTHO;
            if(newIsPerspective && !isPerspective)
                app.m_pCam->m_projectionType = Camera::PERSPECTIVE;
           
            
            
            
            if(isOrtho)
            {
                ImGui::SliderFloat("left", &app.m_pCam->m_projOrtho.m_left,-500.0f, 0.0f);
                ImGui::SliderFloat("right", &app.m_pCam->m_projOrtho.m_right, 0.0f, 500.0f);
                ImGui::SliderFloat("bottom", &app.m_pCam->m_projOrtho.m_bottom,-500.0f, 0.0f);
                ImGui::SliderFloat("top", &app.m_pCam->m_projOrtho.m_top,0.0f, 500.0f);
                ImGui::SliderFloat("near plane", &app.m_pCam->m_projOrtho.m_nearPlane, 0.0f, 100.0f);
                ImGui::SliderFloat("far plane", &app.m_pCam->m_projOrtho.m_farPlane, 100.0f, 10000.0f);
            }else
            {
                ImGui::SliderFloat("verticalFOV", &app.m_pCam->m_projPerspective.m_degVerticalFOV,0.0f, 90.0f);
                ImGui::SliderFloat("ratio width/height", &app.m_pCam->m_projPerspective.m_ratio, 0.0f, 500.0f);
                ImGui::SliderFloat("near Plane", &app.m_pCam->m_projOrtho.m_bottom,-500.0f, 0.0f);
                ImGui::SliderFloat("far Plane", &app.m_pCam->m_projOrtho.m_top,-500.0f, 0.0f);
            }
                 
            ImGui::TreePop();
    }
}





