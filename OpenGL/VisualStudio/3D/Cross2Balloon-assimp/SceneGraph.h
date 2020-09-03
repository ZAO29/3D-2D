#pragma once

#include <vector>

#include<ZGL/ZGLDrawable.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <assimp/cimport.h>

#include "Material.h"
#include "LoadableMesh.h"


class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	void loadModel(std::string filename);
	void InitMesh(aiMesh* aimesh, LoadableMesh * myMesh);
	void Render();


private :
	std::vector<LoadableMesh*> m_pmeshs;
	std::vector<Material*> m_pmaterials;
};






