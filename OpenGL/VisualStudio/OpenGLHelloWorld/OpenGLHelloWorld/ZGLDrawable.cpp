#include "stdafx.h"

#include "ZGLDrawable.h"

#include <GL/glew.h>

ZGLVAODrawable::ZGLVAODrawable()
{
	 
}


ZGLVAODrawable::~ZGLVAODrawable()
{
}

void ZGLVAODrawable::Init(ZGLVAODrawableParam param)
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
		glVertexAttribPointer(i, stride.m_size, stride.m_type, GL_FALSE,m_stride, (void*) offsetCumul);

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
