#pragma once
#include <ZGL/ZGLDrawable.h>

class LoadableMesh
{
public:
	LoadableMesh();
	~LoadableMesh();

	void Init(ZGLVAOIndexedDrawableParam const & init, unsigned int materialId);
	void Render();

	ZGLIndexedVAODrawable* m_pdrawable;
	unsigned int m_materialId;
};

