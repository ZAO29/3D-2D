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

#include <glm.hpp>


#include "Debugging.h"


struct VertexData
{

	VertexData() {}

	VertexData(glm::vec3 pos,glm::vec2 uv) :
		m_pos(pos),
		m_color(0.),
		m_uv(uv),
		m_alpha(0.)
	{}


	VertexData(glm::vec3 pos, glm::vec3 color, glm::vec2 uv, float alpha) :
		m_pos(pos),
		m_color(color),
		m_uv(uv),
		m_alpha(alpha)
	{}
	//VertexData(float x, float y, float z) :m_pos(x,y,z) {}

	glm::vec3 m_pos;
	glm::vec3 m_color;
	glm::vec2 m_uv;
	float m_alpha;
};




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


struct ZGLVAOIndexedDrawableParam 
{	
	unsigned int m_nbIndices;
	unsigned int* m_pIndices;
	ZGLVAODrawableParam m_param;

};





class ZGLVAODrawable {
public:
    ZGLVAODrawable();
	ZGLVAODrawable(const ZGLVAODrawable& orig) { INTERNALERROR("not implemented"); }
    virtual ~ZGLVAODrawable();
    
    void Init(ZGLVAODrawableParam const & init);
    
    void Render(int method);

	void Destroy();


	unsigned int getNbVertices() const { return m_nbVertex; }
    
protected:
	unsigned int m_VAO;
	unsigned int m_VBO;
    unsigned int m_nbVertex;
	std::vector<ZGLStride> m_strides;
	unsigned int m_stride;
};


void InitQuad(ZGLVAODrawable & quad);

class ZGLIndexedVAODrawable : public ZGLVAODrawable
{
public:
	void Init(ZGLVAOIndexedDrawableParam const & init);
	void Render(int method);
	void Destroy();

	unsigned int getNbIndices() const { return m_nbIndices; };
private :
	unsigned int m_IBO;
	unsigned int m_nbIndices;

};



