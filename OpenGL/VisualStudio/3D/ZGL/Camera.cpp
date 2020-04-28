/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera.cpp
 * Author: nicolas
 * 
 * Created on September 20, 2019, 9:53 AM
 */

#include "stdafx.h"

#include "Camera.h"

#include <iostream>


#define __FILEFUNC__ __FILE__<<__FUNCTION__


#include "WindowEnv.h"

#include <GLFW/glfw3.h>

#include <gtx/transform.hpp>
#include <gtx/rotate_vector.hpp>

#include "Listener.h"
#include "imgui/imgui.h"



float clampAngleDegree(float angle, float maxVal, float minVal)
{   
    float shift = maxVal - minVal;
    
    while(angle >= maxVal)
        angle -= shift;
    
    while(angle < minVal)
        angle+= shift;
    
    return angle;
}


void setPitchAndHeadingRad(glm::vec3 & dir, float   H, float  P)
{

    dir.x = sin(H)*cos(P);
    dir.y = sin(P);
    dir.z = cos(H)*cos(P);
}





 float getPitchRad(glm::vec3 const & dir)
 {
     float val = atan2(dir.y,std::sqrt(dir.x*dir.x + dir.z*dir.z));
     
     
     while( val < PI)
         val += 2*PI;
     
    while( val > PI)
         val -= 2*PI;
     
     
    return val; 
 }
 
 
 float getHeadingRad(glm::vec3 const & dir)
 {
    float val = atan2(dir.x,dir.z);
     
     
    while( val < 0)
         val += 2*PI;
     
    while( val >= 2*PI)
         val -= 2*PI;
       
    return val;
 }


float getPitchDeg(glm::vec3 const &  dir)
{
    float val =  RAD2DEG * getPitchRad(dir);   
   return clampAngleDegree(val,180.0f,-180.0f);
    
            
}


float getHeadingDeg(glm::vec3 const &  dir)
{
    float val = RAD2DEG * getHeadingRad(dir);  
    return clampAngleDegree(val,360.0f,0.0f);
            
}





void setPitchAndHeadingDeg(glm::vec3 & dir, float const  Hdeg, float const  Pdeg)
{
    setPitchAndHeadingRad(dir,DEG2RAD * Hdeg, DEG2RAD * Pdeg);
}







Camera::~Camera() {
}

void Camera::Update(float elapsedTime)
{
		if (Listener::sgetKeyState(GLFW_KEY_UP))
		{
			if (m_angleCtrl)
				m_deltaDegP += elapsedTime *m_speed;
			else
				m_eye += elapsedTime * m_speed * m_direction;

		}

		if (Listener::sgetKeyState(GLFW_KEY_DOWN))
		{
			if (m_angleCtrl)
				m_deltaDegP -= elapsedTime * m_speed;
			else
				m_eye -= elapsedTime * m_speed * m_direction;
		}
		

		if (Listener::sgetKeyState(GLFW_KEY_LEFT))
		{
			if (m_angleCtrl)
			{
				//std::cout << __FILE__ << __FUNCTION__ << " rotate " << std::endl;
				m_deltaDegH += elapsedTime * m_speed;
			}
			else
				m_eye += elapsedTime * m_speed * m_right;

		}

		//if (Listener::sgetKeyState(GLFW_KEY_W))
		//{
			m_deltaDegR += elapsedTime * m_speed/2.;
		//}

		if (Listener::sgetKeyState(GLFW_KEY_X))
		{
			m_deltaDegR -= elapsedTime * m_speed/2.;
		}



		if (Listener::sgetKeyState(GLFW_KEY_RIGHT))
		{
			if (m_angleCtrl)
				m_deltaDegH -= elapsedTime * m_speed;
			else
				m_eye -= elapsedTime * m_speed * m_right;
		}
		

		if (Listener::sgetKeyState(GLFW_KEY_U))
		{
			m_eye += elapsedTime * m_speed * m_up;
		}

		if (Listener::sgetKeyState(GLFW_KEY_D))
		{
			m_eye -= elapsedTime * m_speed * m_up;
		}

		if (Listener::sgetKeyState(GLFW_KEY_A))
		{
			if (!m_angleCtrl)
			{
				m_angleCtrl = true;
				std::cout << __FILEFUNC__ << " angle ctrl" << std::endl;
			}
		}

		if (Listener::sgetKeyState(GLFW_KEY_T))
		{
			if (m_angleCtrl)
			{
				m_angleCtrl = false;
				std::cout << __FILEFUNC__ << " translate ctrl" << std::endl;
			}
		}

		if (Listener::sgetKeyState(GLFW_KEY_R))
		{
			m_eye = m_eyeInit; // degree
			m_direction = m_dirInit; // width / height
			//m_right = glm::vec3(1., 0., 0.);
			//m_up = glm::vec3(0., 1., 0.);

			std::cout << __FILEFUNC__ << " reinitialisation " << std::endl;
		}
		if (Listener::sgetKeyState(GLFW_KEY_F))
		{
			m_speed *= 1.01;
		}
		if (Listener::sgetKeyState(GLFW_KEY_G))
		{
			m_speed /= 1.01;
		}

		bool buttonPressed = Listener::sgetMouseButtonState(GLFW_MOUSE_BUTTON_LEFT) ||
			Listener::sgetMouseButtonState(GLFW_MOUSE_BUTTON_RIGHT);

		if (Listener::sgetMouseButtonState(GLFW_MOUSE_BUTTON_LEFT))
		{
			m_eye += m_speed * m_direction * elapsedTime;
		}

		if (Listener::sgetMouseButtonState(GLFW_MOUSE_BUTTON_RIGHT))
		{
			m_eye -= m_speed * m_direction * elapsedTime;
		}

		double xpos, ypos;
		glfwGetCursorPos(m_pwindow, &xpos, &ypos);

		if (buttonPressed)
		{

			int width, height;
			glfwGetWindowSize(m_pwindow, &width, &height);

			m_deltaDegP = (ypos - m_mousePos.y) / float(height)*m_projPerspective.m_degVerticalFOV*0.05;
			m_deltaDegH = (xpos - m_mousePos.x) / float(width)*m_projPerspective.m_degVerticalFOV*m_projPerspective.m_ratio*0.05;
		}

		m_mousePos.x = xpos;
		m_mousePos.y = ypos;
	
}

void Camera::ImguiDraw()
{

	ImGui::LabelText(" position ", " %f,%f,%f",
		getEyePos().x,
		getEyePos().y,
		getEyePos().z);
	ImGui::LabelText(" direction ", " %f,%f,%f",
		getDirection().x,
		getDirection().y,
		getDirection().z);
	ImGui::LabelText("speed ", " %f", m_speed);

}

/*
 void Camera::OnEvent(XEvent Event)
{
    //std::cout<<__FUNCTION__<<' keycode : '<<keycode<<std::endl; 
        
        WindowEnv* pWindowEnv = WindowEnv::getWindowEnv();
        switch (Event.type) 
        {
            case Expose:
               
                break;

            case ConfigureNotify:
               
                break;

            case MotionNotify:  
                if(m_bmousePressed)
                {
                    glm::vec2 newMousePos = glm::vec2(Event.xbutton.x,Event.xbutton.y);
                    m_deltaDegP = (newMousePos.y - m_mousePos.y)/float(pWindowEnv->getWindowHeight())*m_projPerspective.m_degVerticalFOV*0.05;
                    m_deltaDegH = (newMousePos.x - m_mousePos.x)/float(pWindowEnv->getWindowWidth())*m_projPerspective.m_degVerticalFOV*m_projPerspective.m_ratio*0.05;
                }
            
                break;

            case ButtonPress:
                m_bmousePressed = Event.xbutton.button;
                m_mousePos = glm::vec2(Event.xbutton.x,Event.xbutton.y);
                break;
               
            case ButtonRelease:
                m_bmousePressed = 0;

            case KeyPress:
                OnKeyPressed(Event);
                break;
            

            case KeyRelease:
                break;

        case ClientMessage:
            break;
        }
}*/
 
 
 void Camera::controlDeltaPitch()
 {
     if (m_deltaDegH != 0.0f)
     {
            m_direction = glm::rotate<float>(m_direction,m_deltaDegH,m_up);
            m_right = glm::rotate<float>(m_right,m_deltaDegH,m_up); 
     }

     if( m_deltaDegP)
     {
            m_direction = glm::rotate<float>(m_direction,m_deltaDegP,m_right);
            m_up = glm::rotate<float>(m_up,m_deltaDegP,m_right);
     }

	 if (m_deltaDegR)
	 {
		 m_right = glm::rotate<float>(m_right, m_deltaDegR, m_direction);
		 m_up = glm::rotate<float>(m_up, m_deltaDegR, m_direction);
	 }
 }
 
 
 
glm::mat4 Camera::getProjectionView(){
    if (m_projectionType == PERSPECTIVE)
        return glm::perspective(glm::radians(m_projPerspective.m_degVerticalFOV), m_projPerspective.m_ratio, m_projPerspective.m_nearPlane, m_projPerspective.m_farPlane);
    else
        return glm::ortho(m_projOrtho.m_left, m_projOrtho.m_right, m_projOrtho.m_bottom, m_projOrtho.m_top, m_projOrtho.m_nearPlane, m_projOrtho.m_farPlane);
 }
 
 
 
/*void Camera::KeyCallback(int key, int scancode, int action, int mods)
{
    
	if (action != GLFW_PRESS)
		return;
   


    switch (key) 
    {

        case GLFW_KEY_UP:
            if(m_angleCtrl) 
                m_deltaDegP += m_speed;
            else
                m_eye += m_speed*m_direction;
            
            break;
            
        case GLFW_KEY_DOWN:
            if(m_angleCtrl) 
                m_deltaDegP -= m_speed;
            else
                m_eye -= m_speed*m_direction;
            
            break;
            
        case GLFW_KEY_LEFT:
            if(m_angleCtrl)
                m_deltaDegH += m_speed;
            else
                m_eye += m_speed*m_right;
             
            break;
            
        case GLFW_KEY_RIGHT:
            if(m_angleCtrl)
                m_deltaDegH -= m_speed;
            else
                m_eye -= m_speed*m_right;
             
            break;
            
        case GLFW_KEY_U:
            m_eye += m_speed*m_up;
            break;    
            
        case GLFW_KEY_D:
            m_eye -= m_speed*m_up;
            break;
            
        case GLFW_KEY_A:
            if(!m_angleCtrl)
            {
                m_angleCtrl = true;
                std::cout<<__FILEFUNC__<<" angle ctrl"<<std::endl;
            }
            break;
            
        case GLFW_KEY_T:
           if(m_angleCtrl)
            {
                m_angleCtrl = false;
                std::cout<<__FILEFUNC__<<" translate ctrl"<<std::endl;
            }
            break; 
            
        case GLFW_KEY_R:
            
            m_eye = glm::vec3(0,0,5); // degree
            m_direction = glm::vec3(0,0,-1); // width / height
            m_right = glm::vec3(1.,0.,0.);
            m_up = glm::vec3(0.,1.,0.);
             
            std::cout<<__FILEFUNC__<<" reinitialisation "<<std::endl;

    case GLFW_KEY_F:
        m_eye*=1.1;
        break;
    case GLFW_KEY_G:
        m_eye/=1.1;
        break;
                

       
          
            break;     
           

    }
}*/