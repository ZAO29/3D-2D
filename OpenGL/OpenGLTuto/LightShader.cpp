/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LightShader.cpp
 * Author: nicolas
 * 
 * Created on September 24, 2019, 2:52 PM
 */

#include "LightShader.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <gtc/type_ptr.hpp>


LightShader::LightShader() {
}



LightShader::~LightShader() {
}


bool LightShader::Init()
{
    if (!Shader::Init()) {
        return false;
    }

    if (!LoadShader("Shader//glsl//shader.vs")) {
        return false;
    }

    if (!LoadShader("Shader//glsl//shader.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }
     
     
    SetUniformID(m_MVP_ID,"gMVP");
    SetUniformID(m_WorldMat_ID, "gWorld");
    SetUniformID(m_sampler_ID, "gSampler");
    SetUniformID(m_normalMap_sampler_ID, "gSamplerNormalMap");
    SetUniformID(m_dirLightColor_ID, "gLight.Color");
    SetUniformID(m_dirLightAmbientIntensity_ID, "gLight.AmbientIntensity");
    SetUniformID(m_dirLightDirection_ID, "gLight.DirOrPos");
    SetUniformID(m_dirLightDiffuseIntensity_ID, "gLight.DiffuseIntensity");
    SetUniformID(m_eyePos_ID, "gWorldEyePos");
    SetUniformID(m_dirMatSpecularIntensity_ID,"gSpecular.intensity");
    SetUniformID(m_dirMatSpecularPower_ID,"gSpecular.power");
    SetUniformID(m_isDir_ID,"gLight.isDir");
    SetUniformID(m_debug_ID,"gDebug");
    SetUniformID(m_thresholdBrightness_ID,"gThresholdBrightness");
    
    
    return true;
    
    
    // evaluation faineante : un seul probleme plein de probleme
    /*return  (SetUniformID(m_MVP_ID,"gMVP")&&
             SetUniformID(m_sampler_ID, "gSampler") &&
             SetUniformID(m_dirLightColor_ID,"gLight.Color") &&
             SetUniformID(m_dirLightAmbientIntensity_ID,"gLight.AmbientIntensity") &&
             SetUniformID(m_dirLightDirection_ID,"gLight.Dir") &&
             SetUniformID(m_dirLightDiffuseIntensity_ID,"gLight.DiffuseIntensity"));*/
            ;
}



void LightShader::SetMVP(glm::mat4 const & MVP){
    glUniformMatrix4fv(m_MVP_ID, 1, GL_FALSE, glm::value_ptr(MVP));
}


void LightShader::SetWorldMat(glm::mat4 const & m)
{
    glUniformMatrix4fv(m_WorldMat_ID, 1, GL_FALSE, glm::value_ptr(m));
}


void LightShader::SetColorTextureUnit(unsigned int TextureUnit)
{
     glUniform1i(m_sampler_ID, TextureUnit);
}


void LightShader::SetNormalMapTextureUnit(unsigned int TextureUnit)
{
     glUniform1i(m_normalMap_sampler_ID, TextureUnit);
}


void LightShader::SetLight(Light& light)
{
    glUniform3f(m_dirLightColor_ID,light.Color.x,light.Color.y,light.Color.z);
    glUniform1f(m_dirLightAmbientIntensity_ID,light.AmbientIntensity);
    glUniform3f(m_dirLightDirection_ID,light.DirectionOrPos.x,light.DirectionOrPos.y,light.DirectionOrPos.z);
    glUniform1f(m_dirLightDiffuseIntensity_ID,light.DiffuseIntensity);
    glUniform1i(m_isDir_ID,light.isDir);
    
}


void LightShader::SetWorldEye(glm::vec3 pos)
{
    glUniform3f(m_eyePos_ID,pos.x,pos.y,pos.z);
}

void LightShader::SetTresholdBrightness(float val)
{
    glUniform1f(m_thresholdBrightness_ID,val);
}



void LightShader::SetSpecular(Specular s)
{
     glUniform1f(m_dirMatSpecularIntensity_ID,s.intensity);
     glUniform1f(m_dirMatSpecularPower_ID,s.power);
}

void LightShader::SetDebug(bool debug)
{
     glUniform1i(m_debug_ID,debug);
    
}
