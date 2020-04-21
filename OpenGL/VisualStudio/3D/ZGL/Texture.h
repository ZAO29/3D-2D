/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Texture.h
 * Author: nicolas
 *
 * Created on September 23, 2019, 2:49 PM
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include <string>


class Texture {
public:
    Texture(unsigned int TextureTarget, const std::string& FileName);
    Texture(){}
    Texture(float r,float g, float b);
    
    bool Load();

    void Bind(unsigned int TextureUnit);
    
private:
    std::string m_fileName = "";
    unsigned int m_textureTarget = GL_TEXTURE_2D;
    unsigned int m_textureObj = 0;

    
    int m_width = 0;
    int m_height = 0;

};

#endif /* TEXTURE_H */

