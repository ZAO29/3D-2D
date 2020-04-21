/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Texture.cpp
 * Author: nicolas
 * 
 * Created on September 23, 2019, 2:49 PM
 */

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <cmath>


#include <GL/glew.h>
//#include <GL/gl.h>

//#include <SOIL/SOIL.h>

#include "Texture.h"



Texture::Texture(unsigned int TextureTarget, const std::string& FileName):
m_fileName(FileName),m_textureTarget(TextureTarget)
{
    
}

Texture::Texture(float r,float g, float b)
{
    uint8_t ri = std::round(r*255.f);
    uint8_t gi = std::round(g*255.f);
    uint8_t bi = std::round(b*255.f);
    
    
    std::vector<unsigned char> data = {ri,gi,bi};
    m_width=1;
    m_height=1;
    m_textureTarget = GL_TEXTURE_2D;
    GLuint GLChannel = GL_RGB;
    
    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
    
    glTexImage2D(m_textureTarget, 0, GLChannel, m_width, m_height, 0, GLChannel,
              GL_UNSIGNED_BYTE, &data[0]);
    
   
    
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


/*
bool Texture::Load()
{
    int nbChannel = 0;    
    unsigned char* pdata = SOIL_load_image(m_fileName.c_str(),&m_width,&m_height,&nbChannel,SOIL_LOAD_AUTO);   
    GLuint GLChannel;
    
    switch(nbChannel)
    {
    case 1:
        GLChannel = GL_R;
        break;
    case 2:
        GLChannel = GL_RG;
        break;
    case 3:
        GLChannel = GL_RGB;
        break;
    case 4:
        GLChannel = GL_RGBA;
    default:
        break;
    }
    
    
    if(m_width == 0 || m_height == 0 || nbChannel <=0 || nbChannel > 4)
    {
        
        std::cerr<<std::endl<<"texture invalid size (col,line,channel)  "<<m_width<<"|"<<m_height<<"|"<<nbChannel<<std::endl
                <<"for "<<m_fileName<<std::endl;
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        return false;
    }
    
    
    
    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
    
    glTexImage2D(m_textureTarget, 0, GLChannel, m_width, m_height, 0, GLChannel,
              GL_UNSIGNED_BYTE, pdata);
    
    free(pdata);
    
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return true;
    

}*/



void Texture::Bind(unsigned int TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}



