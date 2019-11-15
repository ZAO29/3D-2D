/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Model.cpp
 * Author: nicolas
 * 
 * Created on September 25, 2019, 9:46 AM
 */

#include "Model.h"

#define GLM_FORCE_RADIANS

#include <gtx/transform.hpp>




Model::Model():m_model(glm::mat4(1.0f)) {
}

Model::Model(const Model& orig) {
}

Model::~Model() {
}


void Model::Scale(float scale)
{
    m_model = glm::scale(glm::vec3(scale))*m_model;
}


void Model::Rotate(float angle,glm::vec3 axis )
{
    m_model = glm::rotate(angle,axis)*m_model;
}


void Model::Translate(glm::vec3 vec)
{
    m_model = glm::translate(vec)*m_model;
}


void Model::SetPosition(glm::vec3 vec)
{
    m_model = glm::translate(vec);
}

