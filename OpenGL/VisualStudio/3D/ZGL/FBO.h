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


#include <vector>



class FBO {
public:
    FBO();
    FBO(const FBO& orig);
    virtual ~FBO();
    
    bool Init(unsigned int WindowWidth, unsigned int WindowHeight, unsigned int nbOfTex = 1);
    

    
    void BindForWriting();
    
    void BindForReading(unsigned int texUnit, unsigned int texId=0);
    
	int getTexID(unsigned int texId = 0) { return m_tex_IDS[0]; }


    void RenderQuad();
    
    static void BindToScreen();
    
private:

   unsigned int m_fbo_ID;
   std::vector<unsigned int> m_tex_IDS = std::vector<unsigned int>(1);
   unsigned int m_depthRenderBuffer_ID;
   unsigned int m_quadMesh_ID;
};

#endif /* FBO_H */

