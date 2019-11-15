/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Material.h
 * Author: nicolas
 *
 * Created on October 1, 2019, 3:35 PM
 */

#ifndef MATERIAL_H
#define MATERIAL_H


#include "Texture.h"

class Material {
public:
    Material();
    Material(const Material& orig);
    virtual ~Material();
    
    Texture* m_pDiffuseTexture = nullptr;
    Texture* m_pNormalTexture  = nullptr;
};

#endif /* MATERIAL_H */

