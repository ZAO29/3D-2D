/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BasicShader.h
 * Author: nicolas
 *
 * Created on December 15, 2019, 8:43 PM
 */

#ifndef BASICSHADER_H
#define BASICSHADER_H

#include <ZGL/Shaders.h>

class BasicShader : public Shader{
public:
    BasicShader();
    BasicShader(const BasicShader& orig);
    virtual ~BasicShader();
    
    virtual bool Init() override;
    
    void SetMVP(glm::mat4 const & MVP);
    
private:
    
    unsigned int m_MVP_ID;

};

#endif /* BASICSHADER_H */

