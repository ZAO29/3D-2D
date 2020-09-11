#include "stdafx.h"

#include "ZGLDrawable.h"

#include <GL/glew.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

ZGLVAODrawable::ZGLVAODrawable()
{
	 
}


ZGLVAODrawable::~ZGLVAODrawable()
{
}

void ZGLVAODrawable::Init(ZGLVAODrawableParam const & param)
{

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, param.m_stride * param.m_nbVertex, param.m_pVertices, GL_STATIC_DRAW);


	m_strides = param.m_strides;
	m_nbVertex = param.m_nbVertex;
	m_stride = param.m_stride;
}

void ZGLVAODrawable::Render(int method)
{

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	unsigned int i = 0;
	unsigned int offsetCumul = 0;
	for (auto& stride : m_strides)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, stride.m_size, stride.m_type, GL_FALSE,m_stride, BUFFER_OFFSET(offsetCumul));

		i++;
		offsetCumul += stride.m_offset;
	}
	
	glDrawArrays(method, 0, m_nbVertex);


	
	for (int j = 0; j < m_strides.size(); j++)
	{
		glDisableVertexAttribArray(j);
	}
}

void ZGLVAODrawable::Destroy()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void ZGLIndexedVAODrawable::Init(ZGLVAOIndexedDrawableParam const & init)
{
	ZGLVAODrawable::Init(init.m_param);

	m_nbIndices = init.m_nbIndices;

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*m_nbIndices, (void*) init.m_pIndices, GL_STATIC_DRAW);
	
}



void ZGLIndexedVAODrawable::Render(int method)
{

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	unsigned int i = 0;
	unsigned int offsetCumul = 0;
	for (auto& stride : m_strides)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, stride.m_size, stride.m_type, GL_FALSE, m_stride, BUFFER_OFFSET(offsetCumul));

		i++;
		offsetCumul += stride.m_offset;
	}

	glDrawElements(method, m_nbIndices, GL_UNSIGNED_INT,0);



	for (int j = 0; j < m_strides.size(); j++)
	{
		glDisableVertexAttribArray(j);
	}
}

void ZGLIndexedVAODrawable::Destroy()
{
	ZGLVAODrawable::Destroy();
	glDeleteBuffers(1, &m_IBO);
	
}
