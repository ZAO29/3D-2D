/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SFXBloom.h
 * Author: nicolas
 *
 * Created on October 11, 2019, 3:47 PM
 */

#ifndef SFXBLOOM_H
#define SFXBLOOM_H

//#include "Texture.h"
//#include "BlurShader.h"
//#include "BlendingBloomShader.h"
//#include "FBO.h"
#include <vector>

class Texture;
class BlurShader;
class BlendingBloomShader;
class FBO;

struct BloomSFXConfig
{
    float m_tresholdBrightNess = 1.0;
    unsigned int m_nbPass = 10;   
};


class SFXBloom {
public:
    SFXBloom();

    virtual ~SFXBloom();
    
    
    void Init();
    
    
    void Render(FBO* pFBO,std::vector<FBO*> pingpongFBOs);
    
    BloomSFXConfig m_config;
    
private:
    BlurShader* m_pBlurShader;
    BlendingBloomShader* m_pBlendingShader;
    Texture* m_pBlackTex;
};

#endif /* SFXBLOOM_H */

