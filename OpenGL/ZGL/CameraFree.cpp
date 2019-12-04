/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CameraFree.cpp
 * Author: nicolas
 * 
 * Created on October 8, 2019, 9:35 AM
 */

#include "CameraFree.h"




CameraFree::CameraFree() {
}



CameraFree::~CameraFree() {
}


glm::mat4 CameraFree::getView()
{
    m_deltaDegH = - m_deltaDegH; 
    Camera::controlDeltaPitch();
     
     

     m_deltaDegP = 0.;
     m_deltaDegH = 0.;
     
     if(m_bmousePressed == 1)
     {
         m_eye += m_speed * m_direction;
     }
     
     if(m_bmousePressed == 3)
     {
         m_eye -= m_speed * m_direction;
     }
     
     
     return glm::lookAt(m_eye,m_eye+m_direction,m_up);
     
}

