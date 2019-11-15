/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FBO.h
 * Author: nicolas
 *
 * Created on October 10, 2019, 9:31 AM
 */

#ifndef FBO_H
#define FBO_H

//#include <GL/glew.h>
#include <vector>

typedef unsigned int GLuint;

class FBO {
public:
    FBO();
    FBO(const FBO& orig);
    virtual ~FBO();
    
    bool Init(unsigned int WindowWidth, unsigned int WindowHeight, unsigned int nbOfTex = 0);
    

    
    void BindForWriting();
    
    void BindForReading(unsigned int texUnit, unsigned int texId=0);
    
    void RenderQuad();
    
    static void BindToScreen();
    
private:

   GLuint m_fbo_ID;
   std::vector<GLuint> m_tex_IDS;
   GLuint m_depthRenderBuffer_ID;
   GLuint m_quadMesh_ID;
};

#endif /* FBO_H */

