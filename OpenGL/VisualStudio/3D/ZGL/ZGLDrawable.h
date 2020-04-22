/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZGLVAODrawable.h
 * Author: nicolas
 *
 * Created on December 15, 2019, 8:23 PM
 */

#pragma once



#include <vector>
#include "Debugging.h"



struct ZGLStride
{
	int m_size;
	unsigned int m_type;
	int m_offset;
	
};


struct ZGLVAODrawableParam
{
	int m_stride = 0;
	unsigned int m_nbVertex;
	void * m_pVertices;
	std::vector<ZGLStride> m_strides;
};



class ZGLVAODrawable {
public:
    ZGLVAODrawable();
	ZGLVAODrawable(const ZGLVAODrawable& orig) { INTERNALERROR("not implemented"); }
    virtual ~ZGLVAODrawable();
    
    void Init(ZGLVAODrawableParam init);
    
    void Render(int method);

	void Destroy();
    
private:
	unsigned int m_VAO;
	unsigned int m_VBO;
    unsigned int m_nbVertex;
	std::vector<ZGLStride> m_strides;
	unsigned int m_stride;
};



