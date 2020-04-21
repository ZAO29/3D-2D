#include "ZVBO.h"

#include <GL/glew.h>

ZVBO::ZVBO()
{
}


ZVBO::~ZVBO()
{
}

void ZVBO::Init(std::vector<ZVertex> list)
{
	// VBO : Vertex Buffer Object
	glGenBuffers(1, &_ID);
	glBindBuffer(GL_ARRAY_BUFFER, _ID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
	_nbVertex = list.size();
	glBufferData(GL_ARRAY_BUFFER, sizeof(ZVertex)*_nbVertex, &list[0], GL_STATIC_DRAW);
}

void ZVBO::Render(int method)
{


	//// seulement pour VBO
	//glDrawArrays(GL_TRIANGLES, 0, m_nbVertices);

	//VBO + IBO
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _ID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ZVertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ZVertex), (const GLvoid*)sizeof(glm::vec3));


	glDrawArrays(method, 0, _nbVertex);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}