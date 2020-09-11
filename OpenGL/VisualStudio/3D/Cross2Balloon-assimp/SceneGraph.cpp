#include "stdafx.h"
#include "SceneGraph.h"
#include <ZGL/Debugging.h>
#include <glm.hpp>
#include <ZGL/GLGLEW.h>


#include <ZGL/imgui/imgui.h>
#include <algorithm>

#include <gtc/type_ptr.hpp>


#define SHADER_BB_POS "uBoundingBoxPos"
#define SHADER_BB_SIZE "uBoundingBoxSize"

ZGLVAODrawable* SceneGraph::s_pbbCube = nullptr;
Shader* SceneGraph::s_pbbShader = nullptr;

SceneGraph::SceneGraph()
{
}


SceneGraph::~SceneGraph()
{
}

void SceneGraph::loadModel(std::string filename)
{
	m_name = filename;
	Assimp::Importer importer;
	unsigned int flag = aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_ValidateDataStructure | 0;
	const aiScene* pmodelScene = importer.ReadFile(filename, flag);

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

	for (int i = 0; i < m_pmaterials.size(); i++)
	{
		Material* pmaterial = new Material();
		aiMaterial* aimat = pmodelScene->mMaterials[i];
		InitMaterial(aimat, pmaterial);
		m_pmaterials[i] = pmaterial;

	}



}


struct SGVertex
{

	SGVertex(float x,float y, float z): m_pos(x,y,z){}
	SGVertex(glm::vec3 pos, glm::vec3 normal) : m_pos(pos), m_normal(normal) {}
	SGVertex() :m_pos(0.) {}
	glm::vec3 m_pos;
	glm::vec3 m_normal;
};

void SceneGraph::InitMesh(aiMesh * paiMesh, LoadableMesh * myMesh)
{
	std::vector<SGVertex> vertices;
	vertices.resize(paiMesh->mNumVertices);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		//const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		//vertices[i] = SGVertex(pPos->x/100, pPos->y/100, pPos->z/100);
		vertices[i] = SGVertex(glm::vec3(pPos->x/100,pPos->y/100,pPos->z/100),
							   glm::vec3(pNormal->x,pNormal->y,pNormal->z));


	}


	// BOUNDING BOX CALCULUS
	glm::vec3 minimum,maximum;
	for (int i = 0; i < 3; i++)
	{
		
		auto min_sgv = std::min_element(vertices.begin(),
						 vertices.end(), 
						 [i](const SGVertex & l, const SGVertex & r) {
							return l.m_pos[i] < r.m_pos[i]; 
						  });
		auto max_sgv = std::min_element(vertices.begin(),
			vertices.end(),
			[i](const SGVertex & l, const SGVertex & r) {
			return l.m_pos[i] > r.m_pos[i];
		});

		minimum[i] = min_sgv->m_pos[i];
		maximum[i] = max_sgv->m_pos[i];
	}

	BoundingBox<glm::vec3> bb(minimum, maximum);
	m_bbox.Union(bb);


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

	ZGLStride stride2;
	stride2.m_offset = sizeof(glm::vec3),
	stride2.m_type = GL_FLOAT;
	stride2.m_size = 3;

	paramDrawable.m_param.m_stride = sizeof(SGVertex);
	paramDrawable.m_param.m_nbVertex = static_cast<unsigned int>( vertices.size());
	paramDrawable.m_param.m_pVertices = (void *)&vertices[0];
	paramDrawable.m_param.m_strides = { stride1, stride2 };
	paramDrawable.m_nbIndices = static_cast<unsigned int>( indices.size());
	paramDrawable.m_pIndices = &indices[0];

	myMesh->Init(paramDrawable, paiMesh->mMaterialIndex);

	
}

void SceneGraph::InitMaterial(aiMaterial * aimaterial, Material * mymat)
{
	aiColor3D color;
	if (AI_SUCCESS == aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color))
	{
		mymat->InitDiffuse(color.r, color.g, color.b);
	}
}

void SceneGraph::Render(unsigned int method)
{
	for (auto & pmesh : m_pmeshs)
	{
		pmesh->Render(m_pmaterials, method);
	}
}

void SceneGraph::Destroy()
{
	for (auto & pmesh : m_pmeshs)
	{
		pmesh->Destroy();
		pmesh = nullptr;
	}
	m_pmeshs.resize(0);

	for (auto & pmat : m_pmaterials)
	{
		pmat->Destroy();
		pmat = nullptr;
	}
	m_pmaterials.resize(0);
}

void SceneGraph::ImguiDraw()
{
	int id = 0;

	if(ImGui::TreeNode(m_name.c_str()))
	{
		if (ImGui::TreeNode("meshs"))
		{
			ImGui::Text(" nb meshs : %d", m_pmeshs.size());
			ImGui::TreePop();

			for (auto &pmesh : m_pmeshs)
			{
				
				std::string meshName = "mesh " + std::to_string(id);
				id++;
				if (ImGui::TreeNode(meshName.c_str()))
				{
					pmesh->ImGuiDraw();
					ImGui::TreePop();
				}
			}
		}

		if (ImGui::TreeNode("materials"))
		{
			ImGui::Text(" nb materials : %d", m_pmaterials.size());
			ImGui::TreePop();
		}



		ImGui::TreePop();
	}
	
}

void SceneGraph::RenderBoundingBox(glm::mat4 MVP)
{
	if ((s_pbbCube == nullptr) | (s_pbbShader == nullptr))
	{
		sInitBoundingBoxCube();
	}


	glm::vec3 pos = m_bbox.getCenter<float>();
	glm::vec3 size = m_bbox.getSize();

	s_pbbShader->Enable();
	s_pbbShader->updateUniform(SHADER_MVP, glm::value_ptr(MVP));
	s_pbbShader->updateUniform(SHADER_BB_POS, &pos);
	s_pbbShader->updateUniform(SHADER_BB_SIZE, &size);

	s_pbbCube->Render(GL_LINES);
}

void SceneGraph::sInitBoundingBoxCube()
{
	if (s_pbbCube == nullptr)
	{

		s_pbbCube = new ZGLVAODrawable();

		float a = 0.5f;
		glm::mat3 permutation(0.);
		permutation[1][0] = 1.;
		permutation[2][1] = 1;
		permutation[0][2] = 1;
		// Set up vertex data (and buffer(s)) and attribute pointers
		std::vector<glm::vec3> verticesInit =
		{
			glm::vec3(a, a, a), // Left
			glm::vec3(-a,-a, a),// Right
			glm::vec3(a, -a, a), // Top
			glm::vec3(a, a, a), // Left
			glm::vec3(-a,-a, a),// Right
			glm::vec3(-a, a, a),// Top

			glm::vec3(a, a, -a), // Left
			glm::vec3(-a,-a, -a),// Right
			glm::vec3(a, -a, -a),// Top
			glm::vec3(a, a, -a),// Left
			glm::vec3(-a,-a, -a),// Right
			glm::vec3(-a, a, -a),// Top

		};

		std::vector<glm::vec3> vertices(verticesInit);

		for (int i = 0; i < 2; i++)
		{
			auto vInit = verticesInit;

			for (auto& vert : vInit)
			{
				vert = permutation * vert;
			}

			std::move(vInit.begin(), vInit.end(), std::back_inserter(vertices));

			permutation = glm::transpose(permutation);
		}

		// pos
		ZGLStride stride1;
		stride1.m_offset = sizeof(glm::vec3),
			stride1.m_type = GL_FLOAT;
		stride1.m_size = 3;

		ZGLVAODrawableParam paramDrawable;

		paramDrawable.m_stride = sizeof(glm::vec3);
		paramDrawable.m_nbVertex = static_cast<unsigned int>( vertices.size());
		paramDrawable.m_pVertices = (void *)&vertices[0];
		paramDrawable.m_strides = { stride1 };

		s_pbbCube->Init(paramDrawable);
	}
	

	if (s_pbbShader == nullptr)
	{
		s_pbbShader = new Shader();
		MapUniform uniformMap;
		GraphicPipelineType shaderType;


		uniformMap[SHADER_MVP] = eZGLtypeUniform::ZGL_FMAT4;
		uniformMap[SHADER_BB_POS] = eZGLtypeUniform::ZGL_FVEC3;
		uniformMap[SHADER_BB_SIZE] = eZGLtypeUniform::ZGL_FVEC3;
		s_pbbShader->Init("shaderBBox", uniformMap, shaderType);
	}
}


