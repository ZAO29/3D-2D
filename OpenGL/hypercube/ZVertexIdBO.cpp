/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZVertex4BO.cpp
 * Author: nicolas
 * 
 * Created on December 17, 2019, 9:16 PM
 */

#include "ZVertexIdBO.h"


#include <GL/glew.h>

ZVertex4BO::ZVertex4BO()
{
}

ZVertex4BO::ZVertex4BO(const ZVertex4BO& orig)
{

}

ZVertex4BO::~ZVertex4BO()
{
}

void ZVertex4BO::Init(std::vector<ZVertex4> list)
{
        // VBO : Vertex Buffer Object
	glGenBuffers(1, &_ID);
	glBindBuffer(GL_ARRAY_BUFFER, _ID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
	_nbVertex = list.size();
	glBufferData(GL_ARRAY_BUFFER, sizeof(ZVertex4)*_nbVertex, &list[0], GL_STATIC_DRAW);
}


void ZVertex4BO::Render(int method)
{
    	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _ID);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ZVertex4), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ZVertex4), (const GLvoid*)sizeof(glm::vec4));


	glDrawArrays(method, 0, _nbVertex);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
