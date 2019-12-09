/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SFXBloom.cpp
 * Author: nicolas
 * 
 * Created on October 11, 2019, 3:47 PM
 */

#include "SFXBloom.h"
#include "ZGL/MyConstant.h"

#include "ZGL/Texture.h"
#include "BlurShader.h"
#include "BlendingBloomShader.h"
#include "ZGL/FBO.h"

SFXBloom::SFXBloom() {
}



SFXBloom::~SFXBloom() {
    if(m_pBlackTex != NULL)
    {
        delete m_pBlackTex;
        m_pBlackTex = NULL;
    }
    
    if(m_pBlurShader)
    {
        delete m_pBlurShader;
        m_pBlurShader = NULL;
    }
    
    if(m_pBlendingShader)
    {
        delete m_pBlendingShader;
        m_pBlendingShader = NULL;
    }
}

void SFXBloom::Init()
{
    m_pBlurShader = new BlurShader();
    m_pBlurShader->Init();
    m_pBlurShader->Enable();
    m_pBlurShader->SetRenderedTextureUnit(COLOR_UNIT);
    
    
    m_pBlendingShader = new BlendingBloomShader();
    m_pBlendingShader->Init();
    m_pBlendingShader->Enable();
    m_pBlendingShader->SetBlurrTextureUnit(COLOR_UNIT);
    m_pBlendingShader->SetRenderTextureUnit(NORMALMAP_UNIT);
    m_pBlackTex = new Texture(0,0,0);
}


void SFXBloom::Render(FBO* pFBO,std::vector<FBO*> pingpongFBOs)
{
    pFBO->BindForReading(GL_TEXTURE0,1);
    
    
    // BLURR MULTIPLE PASS
    m_pBlurShader->Enable();
    
    bool horizontal = true;
    for (unsigned int i=0;i< m_config.m_nbPass*2;i++)
    {
        horizontal= !(horizontal);
        m_pBlurShader->SetHorizontal(horizontal);
        
        pingpongFBOs[horizontal]->BindForWriting();
        pingpongFBOs[horizontal]->RenderQuad();
        pingpongFBOs[horizontal]->BindForReading(GL_TEXTURE0);
        
        
    }
    
    //BLENDING 
    glDisable(GL_DEPTH_TEST);
    FBO::BindToScreen();
    m_pBlendingShader->Enable();
    m_pBlendingShader->SetBlurrTextureUnit(0);
    m_pBlendingShader->SetRenderTextureUnit(1);
    pFBO->BindForReading(GL_TEXTURE1,0); // rendu de la scene
    if(m_config.m_nbPass > 0)
        pingpongFBOs[horizontal]->BindForReading(GL_TEXTURE0); // blurred light
    else
        m_pBlackTex->Bind(GL_TEXTURE0);
    pFBO->RenderQuad();
}

