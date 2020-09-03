#pragma once

#include "ZGL/Texture.h"

class Material
{
public:
	Material();
	~Material();

	void InitDiffuse(float r, float g, float b);

private:
	Texture * m_pDiffuseTex;
};

