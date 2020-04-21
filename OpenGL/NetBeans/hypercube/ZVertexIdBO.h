/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZVertex4BO.h
 * Author: nicolas
 *
 * Created on December 17, 2019, 9:16 PM
 */

#ifndef ZVERTEXIDBO_H
#define ZVERTEXIDBO_H

#include <glm/glm.hpp>
#include <vector>

struct ZVertex4
{
    glm::vec4 _pos;
    glm::vec2 _uv;
   
    ZVertex4(glm::vec4 pos,glm::vec2 uv):_pos(pos),_uv(uv){}

};


class ZVertex4BO {
public:
    ZVertex4BO();
    ZVertex4BO(const ZVertex4BO& orig);
    virtual ~ZVertex4BO();
    
    void Init(std::vector<ZVertex4> list);
    
    void Render(int method);
private:
    unsigned int _ID;
    unsigned int _nbVertex;

};

#endif /* ZVERTEXIDBO_H */

