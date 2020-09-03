#include "stdafx.h"
#include "SceneGraph.h"
#include <ZGL/Debugging.h>
#include <glm.hpp>
#include <ZGL/GLGLEW.h>





SceneGraph::SceneGraph()
{
}


SceneGraph::~SceneGraph()
{
}

void SceneGraph::loadModel(std::string filename)
{
	Assimp::Importer importer;
	const aiScene* pmodelScene = importer.ReadFile(filename, aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_ValidateDataStructure | 0

	);

	if (!pmodelScene)
	{
		INTERNALERROR("empty scene");
	}


	m_pmeshs.resize(pmodelScene->mNumMeshes);
	m_pmaterials.resize(pmodelScene->mNumMaterials);

	for (int i = 0; i < m_pmeshs.size(); i++)
	{
		LoadableMesh* pmymesh = new LoadableMesh();
		aiMesh* mesh = pmodelScene->mMeshes[i];
		InitMesh(mesh, pmymesh);
		m_pmeshs[i] = pmymesh;

	}



}


struct SGVertex
{
	SGVertex(float x,float y, float z): m_pos(x,y,z){}
	SGVertex() :m_pos(0.) {}
	glm::vec3 m_pos;
};

void SceneGraph::InitMesh(aiMesh * paiMesh, LoadableMesh * myMesh)
{
	std::vector<SGVertex> vertices;
	vertices.resize(paiMesh->mNumVertices);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		//const aiVector3D* pNormal = &(paiMesh->mNormals[i]) : &Zero3D;
		//const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		vertices[i] = SGVertex(pPos->x/100, pPos->y/100, pPos->z/100);

	}
	
	std::vector<unsigned int> indices;
	indices.resize(paiMesh->mNumFaces*3);
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		aiFace face = paiMesh->mFaces[i];
		if (face.mNumIndices != 3)
		{
			INTERNALERROR("all faces must be triangles");
		}

		for (int k = 0; k < 3; k++)
		{
			if (face.mIndices[k] < vertices.size())
			{
				indices[i * 3 + k] = face.mIndices[k];
			}
			else
			{
				INTERNALERROR(" face indice exceed number of vertices");
			}
			
			
		}
		
	}

	ZGLVAOIndexedDrawableParam paramDrawable;

	// pos
	ZGLStride stride1;
	stride1.m_offset = sizeof(glm::vec3),
	stride1.m_type = GL_FLOAT;
	stride1.m_size = 3;

	paramDrawable.m_param.m_stride = sizeof(SGVertex);
	paramDrawable.m_param.m_nbVertex = vertices.size();
	paramDrawable.m_param.m_pVertices = (void *)&vertices[0];
	paramDrawable.m_param.m_strides = { stride1 };
	paramDrawable.m_nbIndices = indices.size();
	paramDrawable.m_pIndices = &indices[0];

	myMesh->Init(paramDrawable, paiMesh->mMaterialIndex);

	
}

void SceneGraph::Render()
{
	for (auto & pmesh : m_pmeshs)
	{
		pmesh->Render();
	}
}


