/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BlendingBloomShader.cpp
 * Author: nicolas
 * 
 * Created on October 11, 2019, 11:01 AM
 */

#include "BlendingBloomShader.h"

#include <GL/glew.h>

BlendingBloomShader::BlendingBloomShader() {
}

BlendingBloomShader::BlendingBloomShader(const BlendingBloomShader& orig) {
}

BlendingBloomShader::~BlendingBloomShader() {
}

bool BlendingBloomShader::Init()
{
    if (!Shader::Init()) {
        return false;
    }

    if (!LoadShader("Shader//glsl//postFXpassthrough.vs")) {
        return false;
    }

    if (!LoadShader("Shader//glsl//BlendingTexture.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }
    
    SetUniformID(m_RenderedTex_ID, "gScene");
    SetUniformID(m_blurredTex_ID, "gBloomBlur");
    
    return true;
}


 void BlendingBloomShader::SetBlurrTextureUnit(unsigned int TextureUnit)
 {
     glUniform1i(m_blurredTex_ID, TextureUnit);
 }
 
 void BlendingBloomShader::SetRenderTextureUnit(unsigned int TextureUnit)
 {
     glUniform1i(m_RenderedTex_ID, TextureUnit);
 }

