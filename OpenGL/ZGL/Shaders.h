/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Shaders.h
 * Author: nicolas
 *
 * Created on September 18, 2019, 4:12 PM
 */

#ifndef SHADERS_H
#define SHADERS_H


#include <list>
#include <string>

 #define GLM_FORCE_RADIANS

#include <glm.hpp>






class Shader {
public:
    Shader();
    virtual ~Shader();
    
    
    
    
    
    virtual bool Init();

    void Enable();
    
 
        

    
protected :
    

    
    bool LoadShader(const char * file_path);
    
    bool Finalize();
    
    bool SetUniformID(unsigned int& ID,std::string shader_var);

    
    

    
private:
    
    // classe mere
    unsigned int m_shaderProg;
    std::list<unsigned int> m_shaderObjList;


};

#endif /* SHADERS_H */

