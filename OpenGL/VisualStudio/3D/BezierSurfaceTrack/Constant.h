#pragma once
#include "glm.hpp"






struct VertexData
{

	VertexData() {}

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