/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BlurShader.cpp
 * Author: nicolas
 * 
 * Created on October 10, 2019, 9:52 AM
 */

#include "BlurShader.h"


#include <GL/glew.h>
#include <GL/gl.h>
#include <gtc/type_ptr.hpp>

BlurShader::BlurShader() {
}

BlurShader::BlurShader(const BlurShader& orig) {
}

BlurShader::~BlurShader() {
}


bool BlurShader::Init()
{
    if (!Shader::Init()) {
        return false;
    }

    if (!LoadShader("Shader//glsl//postFXpassthrough.vs")) {
        return false;
    }

    if (!LoadShader("Shader//glsl//blurFX.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }
     
     
    
    SetUniformID(m_renderedTex_ID, "gRenderedTex");
    SetUniformID(m_horizontal_ID, "gHorizontal");
   
    
    
    return true;
    
}

void BlurShader::SetRenderedTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_renderedTex_ID, TextureUnit);
}


void BlurShader::SetHorizontal(bool isHorizontal)
{
    glUniform1i(m_horizontal_ID, isHorizontal);
}
