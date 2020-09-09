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
#include <ZGL/BoundingBox.h>
#include <ZGL/Shaders.h>


class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	void loadModel(std::string filename);
	void InitMesh(aiMesh* aimesh, LoadableMesh * myMesh);
	void InitMaterial(aiMaterial* aimaterial, Material * mymaterial);
	void Render(unsigned int method);
	void Destroy();
	void ImguiDraw();
	void RenderBoundingBox(glm::mat4 MVP);
	static void sInitBoundingBoxCube();

private :
	std::vector<LoadableMesh*> m_pmeshs;
	std::vector<Material*> m_pmaterials;
	std::string m_name;
	bool m_bshawMesh;
	bool m_bshawMat;


    
	static ZGLVAODrawable* s_pbbCube;
	static Shader* s_pbbShader;

public:
	BoundingBox<glm::vec3> m_bbox = BoundingBox<glm::vec3>();
};






