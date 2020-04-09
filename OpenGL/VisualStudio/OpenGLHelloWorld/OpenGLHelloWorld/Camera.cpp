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

void Camera::KeyCallback(int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		std::cout << __FUNCTION__ << "KEY E pressed" << std::endl;
	}

	switch (key)
	{

	case GLFW_KEY_UP:
		if (m_angleCtrl)
			m_deltaDegP += m_speed;
		else
			m_eye += m_speed * m_direction;

		break;

	case GLFW_KEY_DOWN:
		if (m_angleCtrl)
			m_deltaDegP -= m_speed;
		else
			m_eye -= m_speed * m_direction;

		break;

	case GLFW_KEY_LEFT:
		if (m_angleCtrl)
			m_deltaDegH += m_speed;
		else
			m_eye += m_speed * m_right;

		break;

	case GLFW_KEY_RIGHT:
		if (m_angleCtrl)
			m_deltaDegH -= m_speed;
		else
			m_eye -= m_speed * m_right;

		break;

	case GLFW_KEY_U:
		m_eye += m_speed * m_up;
		break;

	case GLFW_KEY_D:
		m_eye -= m_speed * m_up;
		break;

	case GLFW_KEY_A:
		if (!m_angleCtrl)
		{
			m_angleCtrl = true;
			std::cout << __FILEFUNC__ << " angle ctrl" << std::endl;
		}
		break;

	case GLFW_KEY_T:
		if (m_angleCtrl)
		{
			m_angleCtrl = false;
			std::cout << __FILEFUNC__ << " translate ctrl" << std::endl;
		}
		break;

	case GLFW_KEY_R:

		m_eye = glm::vec3(0, 0, 5); // degree
		m_direction = glm::vec3(0, 0, -1); // width / height
		m_right = glm::vec3(1., 0., 0.);
		m_up = glm::vec3(0., 1., 0.);

		std::cout << __FILEFUNC__ << " reinitialisation " << std::endl;

	case GLFW_KEY_F:
		m_eye *= 1.1;
		break;
	case GLFW_KEY_G:
		m_eye /= 1.1;
		break;




		break;


	}
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
 }
 
 
 
glm::mat4 Camera::getProjectionView(){
    if (m_projectionType == PERSPECTIVE)
        return glm::perspective(glm::radians(m_projPerspective.m_degVerticalFOV), m_projPerspective.m_ratio, m_projPerspective.m_nearPlane, m_projPerspective.m_farPlane);
    else
        return glm::ortho(m_projOrtho.m_left, m_projOrtho.m_right, m_projOrtho.m_bottom, m_projOrtho.m_top, m_projOrtho.m_nearPlane, m_projOrtho.m_farPlane);
 }
 
 
 /*
 void Camera::OnKeyPressed(XEvent event)
{
    int32_t keycode = XLookupKeysym(&event.xkey, 0);
    unsigned int windowFlag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;


    switch (keycode) 
    {

        case XK_Up:
            if(m_angleCtrl) 
                m_deltaDegP += m_speed;
            else
                m_eye += m_speed*m_direction;
            
            break;
            
        case XK_Down:
            if(m_angleCtrl) 
                m_deltaDegP -= m_speed;
            else
                m_eye -= m_speed*m_direction;
            
            break;
            
        case XK_Left:
            if(m_angleCtrl)
                m_deltaDegH += m_speed;
            else
                m_eye += m_speed*m_right;
             
            break;
            
        case XK_Right:
            if(m_angleCtrl)
                m_deltaDegH -= m_speed;
            else
                m_eye -= m_speed*m_right;
             
            break;
            
        case XK_u:
            m_eye += m_speed*m_up;
            break;    
            
        case XK_d:
            m_eye -= m_speed*m_up;
            break;
            
        case XK_a:
            if(!m_angleCtrl)
            {
                m_angleCtrl = true;
                std::cout<<__FILEFUNC__<<" angle ctrl"<<std::endl;
            }
            break;
            
        case XK_t:
           if(m_angleCtrl)
            {
                m_angleCtrl = false;
                std::cout<<__FILEFUNC__<<" translate ctrl"<<std::endl;
            }
            break; 
            
        case XK_r:
            
            m_eye = glm::vec3(0,0,5); // degree
            m_direction = glm::vec3(0,0,-1); // width / height
            m_right = glm::vec3(1.,0.,0.);
            m_up = glm::vec3(0.,1.,0.);
             
            std::cout<<__FILEFUNC__<<" reinitialisation "<<std::endl;

    case XK_f:
        m_eye*=1.1;
        break;
    case XK_g:
        m_eye/=1.1;
        break;
                

       
          
            break;     
           

    }
}*/