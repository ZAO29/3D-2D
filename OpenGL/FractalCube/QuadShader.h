/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   QuadShader.h
 * Author: nicolas
 *
 * Created on December 16, 2019, 7:30 PM
 */

#ifndef QUADSHADER_H
#define QUADSHADER_H

#include <ZGL/Shaders.h>

class QuadShader : public Shader{
public:
    QuadShader();
    QuadShader(const QuadShader& orig);
    virtual ~QuadShader();
    
    bool Init() override;
private:

};

#endif /* QUADSHADER_H */

