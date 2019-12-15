/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LightShader.h
 * Author: nicolas
 *
 * Created on September 24, 2019, 2:52 PM
 */

#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

#include "ZGL/Shaders.h"


struct Light
{
    glm::vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
    glm::vec3 DirectionOrPos;
    bool isDir = true;
};



struct Specular
{
    float intensity = 1.0;
    float power = 5.0;
};




class LightShader : public  Shader
{
public:
    LightShader();
    virtual ~LightShader();
    
    void SetMVP(glm::mat4 const & MVP);
    void SetWorldMat(glm::mat4 const &  m);
    void SetColorTextureUnit(unsigned int TextureUnit);
    void SetNormalMapTextureUnit(unsigned int TextureUnit);
    void SetLight(Light& light);
    void SetWorldEye(glm::vec3 pos);
    void SetSpecular(Specular);
    void SetDebug(bool debug);
    void SetTresholdBrightness(float val);
    
    
    virtual bool Init() override;
    
private:
    
    //specifique
    unsigned int m_MVP_ID;
    unsigned int m_WorldMat_ID;
    unsigned int m_sampler_ID;
    unsigned int m_normalMap_sampler_ID;
    
    
    unsigned int m_dirLightColor_ID;
    unsigned int m_dirLightAmbientIntensity_ID;
    unsigned int m_dirLightDiffuseIntensity_ID;
    unsigned int m_dirLightDirection_ID;
    unsigned int m_eyePos_ID;
    unsigned int m_isDir_ID;
    
    unsigned int m_dirMatSpecularIntensity_ID;
    unsigned int m_dirMatSpecularPower_ID;
    
    
    unsigned int m_debug_ID;
    unsigned int m_thresholdBrightness_ID;
    
};

#endif /* LIGHTSHADER_H */

