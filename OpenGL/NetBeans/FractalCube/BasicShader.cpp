/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BasicShader.cpp
 * Author: nicolas
 * 
 * Created on December 15, 2019, 8:43 PM
 */

#include "BasicShader.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <gtc/type_ptr.hpp>


BasicShader::BasicShader()
{
}

BasicShader::BasicShader(const BasicShader& orig)
{
}

BasicShader::~BasicShader()
{
}

bool BasicShader::Init()
{
    Shader::Init("shader",false);
     
    SetUniformID(m_MVP_ID,"gMVP");

    
    return true;
}

void BasicShader::SetMVP(glm::mat4 const & MVP){
    glUniformMatrix4fv(m_MVP_ID, 1, GL_FALSE, glm::value_ptr(MVP));
}
