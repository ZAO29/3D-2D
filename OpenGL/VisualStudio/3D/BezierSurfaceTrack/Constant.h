#pragma once
#include "glm.hpp"






struct VertexData
{

	VertexData() {}

	VertexData(glm::vec3 pos, glm::vec3 color, float alpha) :
		m_pos(pos),
		m_color(color),
		m_alpha(alpha)
	{}
	//VertexData(float x, float y, float z) :m_pos(x,y,z) {}

	glm::vec3 m_pos;
	glm::vec3 m_color;
	float m_alpha;
};