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


#include <string>


struct TexParam
{
	TexParam(int width, int height, int channel, int type):
	m_width(width),
	m_height(height),
	m_channel(channel),
		m_type(type)
	{}
	TexParam() {}
	int m_width = 0;
	int m_height = 0;
	int m_channel = 0x1908; // GL_RBGA
	int m_type = 0; // GL_UNSIGNED BYTE

};


class Texture {
public:
    Texture(unsigned int TextureTarget, const std::string& FileName);
    Texture(){}
    Texture(float r,float g, float b);

	void Init(TexParam param, void* data=NULL);

	void update(void* data);

	TexParam getParam() { return m_param; }

	int getID() { return m_textureObj; }

	void getData(void* data);
    

	void Destroy();
    virtual bool Load(std::string file);

    void Bind(unsigned int TextureUnit);
    
	protected:
    std::string m_fileName = "";
		
    unsigned int m_textureTarget = 0x0DE1; //GL_TEXTURE_2D
    unsigned int m_textureObj = 0;

	TexParam m_param;
    

};

#endif /* TEXTURE_H */

