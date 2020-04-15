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

#include "stdafx.h"

#include "CameraFree.h"
#include "Listener.h"





CameraFree::CameraFree(GLFWwindow* pwin):Camera(pwin)
{
	m_eye = glm::vec3(20, 0., 0.);
	m_direction = glm::vec3(1., 0., 0.);
	m_right = glm::vec3(0., 1., 0.);
	m_up = glm::vec3(0., 0., 1.);
}



CameraFree::~CameraFree() {
}


glm::mat4 CameraFree::getView()
{
    m_deltaDegH = - m_deltaDegH; 
    Camera::controlDeltaPitch();
     
     

     m_deltaDegP = 0.;
     m_deltaDegH = 0.;
     
     
     
     
     return glm::lookAt(m_eye,m_eye+m_direction,m_up);
     
}

void CameraFree::Update(float elapsedTime)
{
	Camera::Update(elapsedTime);

}

