#include "stdafx.h"
#include "LoadableMesh.h"
#include "ZGL/GLGLEW.h"
#include "ZGL/Debugging.h"
#include <ZGL/imgui/imgui.h>

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
	m_materialId = materialId;
}

void LoadableMesh::Render(std::vector<Material*> & materials, unsigned int method)
{
	if (m_materialId > materials.size() ||
		materials[m_materialId] == NULL)
	{
		INTERNALERROR("invalid material id");
	}
	materials[m_materialId]->Bind();
	m_pdrawable->Render(method);
}

void LoadableMesh::Destroy()
{
	m_pdrawable->Destroy();
	m_pdrawable = nullptr;
}

void LoadableMesh::ImGuiDraw()
{
	int nbid = m_pdrawable->getNbIndices();
	int nb_vertices = m_pdrawable->getNbVertices();
	ImGui::Text("nb vertices : %d", nb_vertices);
	ImGui::Text("nb  indices: %d", nbid);
	ImGui::Text("material id: %u", m_materialId);
}
