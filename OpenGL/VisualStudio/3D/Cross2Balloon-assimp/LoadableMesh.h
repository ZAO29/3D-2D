#pragma once
#include <ZGL/ZGLDrawable.h>
#include "Material.h"

class LoadableMesh
{
public:
	LoadableMesh();
	~LoadableMesh();

	void Init(ZGLVAOIndexedDrawableParam const & init, unsigned int materialId);
	void Render(std::vector<Material*> & materials);
	void Destroy();
	void ImGuiDraw();
	ZGLIndexedVAODrawable* m_pdrawable;
	unsigned int m_materialId;
};

