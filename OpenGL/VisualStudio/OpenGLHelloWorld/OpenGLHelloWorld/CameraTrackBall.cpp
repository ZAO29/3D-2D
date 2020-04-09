/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CameraTrackBall.cpp
 * Author: nicolas
 * 
 * Created on October 7, 2019, 5:11 PM
 */
#include "stdafx.h"

#include "CameraTrackBall.h"

// GLM
#include <gtx/transform.hpp>
#include <gtx/rotate_vector.hpp>




CameraTrackBall::CameraTrackBall() : Camera() {
}



CameraTrackBall::~CameraTrackBall() {
}


glm::mat4 CameraTrackBall::getView()
{
     if (m_deltaDegH != 0.0f)
     {
            m_eye = glm::rotate<float>(m_eye,m_deltaDegH,m_up);
            m_right = glm::rotate<float>(m_right,m_deltaDegH,m_up); 
     }

     if( m_deltaDegP)
     {
            m_eye = glm::rotate<float>(m_eye,m_deltaDegP,m_right);
            m_up = glm::rotate<float>(m_up,m_deltaDegP,m_right);
     }
     
     
     m_deltaDegP = 0.;
     m_deltaDegH = 0.;
     m_direction = -glm::normalize(m_eye);
    return glm::lookAt(m_eye,glm::vec3(0,0,0),m_up);
}









