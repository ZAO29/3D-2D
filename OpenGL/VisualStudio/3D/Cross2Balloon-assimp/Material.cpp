#include "stdafx.h"
#include "Material.h"
#include "ZGL/GLGlew.h"

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

void Material::Bind()
{
	m_pDiffuseTex->Bind(GL_TEXTURE0);
}

void Material::Destroy()
{
	m_pDiffuseTex->Destroy();
	m_pDiffuseTex = nullptr;
}
