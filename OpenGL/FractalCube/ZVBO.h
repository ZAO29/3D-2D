/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZVBO.h
 * Author: nicolas
 *
 * Created on December 15, 2019, 8:23 PM
 */

#ifndef ZVBO_H
#define ZVBO_H


#include <glm/glm.hpp>
#include <vector>

struct ZVertex
{
    glm::vec3 _pos;
    glm::vec2 _uv;
   
    ZVertex(glm::vec3 pos,glm::vec2 uv):_pos(pos),_uv(uv){}

};



class ZVBO {
public:
    ZVBO();
    ZVBO(const ZVBO& orig);
    virtual ~ZVBO();
    
    void Init(std::vector<ZVertex> list);
    
    void Render(int method);
    
private:
    unsigned int _ID;
    unsigned int _nbVertex;
};

#endif /* ZVBO_H */

