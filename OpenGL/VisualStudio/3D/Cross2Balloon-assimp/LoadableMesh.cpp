#include "stdafx.h"
#include "LoadableMesh.h"
#include "ZGL/GLGLEW.h"

LoadableMesh::LoadableMesh()
{
	m_pdrawable = new ZGLIndexedVAODrawable();
}


LoadableMesh::~LoadableMesh()
{
}

void LoadableMesh::Init(ZGLVAOIndexedDrawableParam const & init, unsigned int materialId)
{
	m_pdrawable->Init(init);
	m_materialId = m_materialId;
}

void LoadableMesh::Render()
{
	m_pdrawable->Render(GL_TRIANGLES);
}
