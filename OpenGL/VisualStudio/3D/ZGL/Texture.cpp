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


#include "GLGLEW.h"

//#include <SOIL/SOIL.h>

#include "Texture.h"
#include "Debugging.h"



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

    
	TexParam param = TexParam(1, 1, GL_RGB, GL_UNSIGNED_BYTE);
    
   
	Init(param, &data[0]);
}

void Texture::Init(TexParam param,void* data)
{
	m_param = param;


	m_textureTarget = GL_TEXTURE_2D;


	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	int levelmap = 0;
	glTexImage2D(m_textureTarget, levelmap, m_param.m_channel, m_param.m_width, m_param.m_height, 0, m_param.m_channel,
		m_param.m_type, data);



	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::update(void * data)
{
	if (m_textureObj == 0)
	{
		INTERNALERROR(" tex not created");
	}
		

	glBindTexture(m_textureTarget, m_textureObj);
	int levelmap = 0;
	glTexImage2D(m_textureTarget, levelmap, m_param.m_channel, m_param.m_width, m_param.m_height, 0, m_param.m_channel,
		m_param.m_type, data);
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



void Texture::getData(void * data)
{
	Bind(GL_TEXTURE0);

	glGetTexImage(m_textureTarget, 0, m_param.m_channel, m_param.m_type, data);
}

void Texture::Destroy()
{
	glDeleteTextures(1, &m_textureObj);
}

void Texture::Bind(unsigned int TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}



