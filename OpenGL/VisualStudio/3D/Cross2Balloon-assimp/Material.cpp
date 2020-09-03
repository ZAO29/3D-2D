#include "stdafx.h"
#include "Material.h"


Material::Material()
{
}


Material::~Material()
{
}

void Material::InitDiffuse(float r, float g, float b)
{
	m_pDiffuseTex = new Texture(r,g,b);
}
