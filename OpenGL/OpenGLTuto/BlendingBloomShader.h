/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BlendingBloomShader.h
 * Author: nicolas
 *
 * Created on October 11, 2019, 11:01 AM
 */

#ifndef BLENDINGBLOOMSHADER_H
#define BLENDINGBLOOMSHADER_H

#include "ZGL/Shaders.h"

class BlendingBloomShader : public Shader {
public:
    BlendingBloomShader();
    BlendingBloomShader(const BlendingBloomShader& orig);
    virtual ~BlendingBloomShader();
    
    bool Init();
    
    void SetBlurrTextureUnit(unsigned int TextureUnit);
    void SetRenderTextureUnit(unsigned int TextureUnit);
    
private:

    unsigned int m_blurredTex_ID;
    unsigned int m_RenderedTex_ID;
    
};

#endif /* BLENDINGBLOOMSHADER_H */

