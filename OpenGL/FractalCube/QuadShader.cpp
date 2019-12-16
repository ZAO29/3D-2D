/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   QuadShader.cpp
 * Author: nicolas
 * 
 * Created on December 16, 2019, 7:30 PM
 */

#include "QuadShader.h"

QuadShader::QuadShader()
{
}

QuadShader::QuadShader(const QuadShader& orig)
{
}

QuadShader::~QuadShader()
{
}

 bool QuadShader::Init()
 {
     if (!Shader::Init()) {
        return false;
    }

    if (!LoadShader("glsl//quadShader.vs")) {
        return false;
    }

    if (!LoadShader("glsl//quadShader.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }
     
     return true;
     
}