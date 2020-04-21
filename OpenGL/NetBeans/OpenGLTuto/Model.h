/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Model.h
 * Author: nicolas
 *
 * Created on September 25, 2019, 9:46 AM
 */

#ifndef MODEL_H
#define MODEL_H


#include "glm.hpp"

class Model {
public:
    Model();
    Model(const Model& orig);
    
    void Rotate(float angle,glm::vec3 axis );
    
    void Translate(glm::vec3 vec);
    
    void SetPosition(glm::vec3 vec);
    
    void Scale(float scale);
    
    const glm::mat4& getMat(){return m_model;}
    virtual ~Model();
private:
    
    glm::mat4 m_model;

};

#endif /* MODEL_H */

