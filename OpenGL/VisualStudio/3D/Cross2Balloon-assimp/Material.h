#pragma once

#include "ZGL/Texture.h"

class Material
{
public:
	Material();
	~Material();

	void InitDiffuse(float r, float g, float b);
	void Bind();
	void Destroy();

private:
	Texture * m_pDiffuseTex;
};

