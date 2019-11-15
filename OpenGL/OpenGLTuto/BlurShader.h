/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BlurShader.h
 * Author: nicolas
 *
 * Created on October 10, 2019, 9:52 AM
 */

#ifndef POSTFX1SHADER_H
#define POSTFX1SHADER_H
#include "Shaders.h"

class BlurShader : public Shader {
public:
    BlurShader();
    BlurShader(const BlurShader& orig);
    virtual ~BlurShader();
    
    bool Init();
    
    void SetRenderedTextureUnit(unsigned int TextureUnit);
    
    void SetHorizontal(bool isHorizontal);
    
private:
    
    unsigned int m_renderedTex_ID;
    unsigned int m_horizontal_ID;

};

#endif /* POSTFX1SHADER_H */

