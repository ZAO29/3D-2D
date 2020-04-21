/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>

    // assimp


#include "Mesh.h"
#include "ZGL/MyConstant.h"
#include <iostream>
#include <fstream>

#include <GL/gl.h>

#include <Importer.hpp>  

Mesh::MeshEntry::MeshEntry()
{
    VB = INVALID_OGL_VALUE;
    IB = INVALID_OGL_VALUE;
    NumIndices  = 0;
    MaterialIndex = INVALID_MATERIAL;
};

Mesh::MeshEntry::~MeshEntry()
{
    if (VB != INVALID_OGL_VALUE)
    {
        glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_OGL_VALUE)
    {
        glDeleteBuffers(1, &IB);
    }
}

void Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices,
                          const std::vector<unsigned int>& Indices)
{
    NumIndices = Indices.size();

    glGenBuffers(1, &VB);
  	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
}

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
    Clear();
}


void Mesh::Clear()
{
    for (unsigned int i = 0 ; i < m_materials.size() ; i++) {
        free(m_materials[i].m_pDiffuseTexture);
        free(m_materials[i].m_pNormalTexture);
    }
}


bool Mesh::LoadMesh(const std::string& Filename,const std::string& textureFolder)
{
    // Release the previously loaded mesh (if it exists)
    Clear();
    std::cout<<std::endl <<__FILEFUNC__<< Filename << std::endl;
    m_importConfig.m_TexFolder = textureFolder;
    
    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | 
                                                                aiProcess_GenSmoothNormals | 
                                                                aiProcess_FlipUVs |
                                                                aiProcess_CalcTangentSpace);
    
    if (pScene) {
        Ret = InitFromScene(pScene, Filename);
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
    }

    return Ret;
}

void Mesh::GetMeshInfo(aiNode* node, unsigned int& nbMesh,BoundingBox& bbox, const aiScene* pScene, aiMatrix4x4 transform)
{
    if(node == NULL)
        return;
    
    nbMesh += node->mNumMeshes;
    
    
    
    transform = transform * node->mTransformation;
    for (int i=0;i < node->mNumMeshes;i++)
    {
        int idMesh = node->mMeshes[i];
        aiMesh* pMesh = pScene->mMeshes[idMesh];
        
        aiVector3D pos;
        
        for (int j=0;j < pMesh->mNumVertices;j++)
        {
            pos = transform * pMesh->mVertices[i];
            
            if(bbox._min.x  > bbox._max.x)
            {
                bbox._min = glm::vec3(pos.x,pos.y,pos.z);
                bbox._max = glm::vec3(pos.x,pos.y,pos.z);
            }
            
            
            if(bbox._min.x > pos.x) bbox._min.x = pos.x;
            if(bbox._max.x < pos.x) bbox._max.x = pos.x;
            
            if(bbox._min.y > pos.y) bbox._min.y = pos.y;
            if(bbox._max.y < pos.y) bbox._max.y = pos.y;
            
            if(bbox._min.z > pos.z) bbox._min.z = pos.z;
            if(bbox._max.z < pos.z) bbox._max.z = pos.z;

          
        }
    }
    
    
    
    for (int i=0;i<node->mNumChildren;i++)
    {
        GetMeshInfo(node->mChildren[i],nbMesh,bbox,pScene,transform);
    }
}


bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{  
    m_materials.resize(pScene->mNumMaterials);

    
    unsigned int index = 0;
    
 
    unsigned int nbMesh = 0;
    BoundingBox bb;
    GetMeshInfo(pScene->mRootNode,nbMesh,bb,pScene,aiMatrix4x4());
    std::cout<<__FUNCTION__<<" | nbMesh : "<<nbMesh<<std::endl;
    std::cout<<__FUNCTION__<<" | bounding box min : "<<bb._min.x<<'|'<< bb._min.y <<'|'<< bb._min.z << std::endl;
    std::cout<<__FUNCTION__<<" | bounding box max : "<<bb._max.x<<'|'<< bb._max.y <<'|'<< bb._max.z << std::endl;
    glm::vec3 diff = bb._max - bb._min;
    std::cout<<__FUNCTION__<<" | bounding box size : "<<diff.x<<'|'<< diff.y <<'|'<< diff.z << std::endl;
    m_Entries.resize(nbMesh);
    
    float boxSizeMean = (diff.x + diff.y + diff.z)/3.0f;
    
    if (boxSizeMean <  m_importConfig.m_minSize)
    {
        m_scale = m_importConfig.m_minSize / boxSizeMean;
    }
    
    
    if (boxSizeMean >  m_importConfig.m_maxSize)
    {
        m_scale = m_importConfig.m_maxSize / boxSizeMean;
    }
    
    if(boxSizeMean == 0.)
    {
        m_scale = 1.0;
    }
      
    
    
    std::cout<<__FUNCTION__<<" | scale : "<< m_scale << std::endl;
    
    
    
    
    aiMatrix4x4 transform;
    aiMatrix4x4::Scaling(aiVector3D(m_scale),transform);
    
    InitMeshsFromNode(pScene->mRootNode,index,pScene,transform);

            
    return InitMaterials(pScene, Filename);
}


void Mesh::InitMeshsFromNode(aiNode* node, unsigned int &index, const aiScene* pScene, aiMatrix4x4 transform)
{
    
    if(node == NULL)
        return;
    
      transform = transform * node->mTransformation;
        
    for (int i=0;i<node->mNumMeshes;i++)
    {
        int idMesh = node->mMeshes[i];
        InitMesh(index,pScene->mMeshes[idMesh],transform);
        index++;        
    }

    for (int i=0;i< node->mNumChildren;i++)
    {
        InitMeshsFromNode(node->mChildren[i],index,pScene,transform);
    }
    
}






void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh,aiMatrix4x4 transform)
{
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
    
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    
    Vertices.resize(paiMesh->mNumVertices);
    
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
         aiVector3D* pPos      = &(paiMesh->mVertices[i]);
         aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        
        
        
        *pPos =  transform * (*pPos);
        *pNormal = transform * (*pNormal);
       
        
        Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
                 glm::vec2(pTexCoord->x, pTexCoord->y),
                 glm::vec3(pNormal->x, pNormal->y, pNormal->z));

        Vertices[i]=v;
    }

    Indices.resize(paiMesh->mNumFaces*3);
    
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices[3*i]= Face.mIndices[0];
        Indices[3*i+1]= Face.mIndices[1];
        Indices[3*i+2]= Face.mIndices[2];
    }

    m_Entries[Index].Init(Vertices, Indices);
}


inline bool exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }
    
    if(m_importConfig.m_TexFolder != "")
    {
        Dir = m_importConfig.m_TexFolder;
    }
    
    std::cout<<__FUNCTION__<<"Texture Dir : "<< Dir << std::endl;
    
    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

       
        // TEXTURE_DIFFUSE
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath;
                if(exists(Dir + Path.data))
                    FullPath = Dir +  Path.data;  
                if(exists(Path.data))
                    FullPath = Path.data;
                
                m_materials[i].m_pDiffuseTexture = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_materials[i].m_pDiffuseTexture->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_materials[i].m_pDiffuseTexture;
                    m_materials[i].m_pDiffuseTexture = NULL;
                    Ret = false;
                }
                else {
                    printf("Loaded Diffuse texture '%s'\n", FullPath.c_str());
                }
            }
        }
              
        if(!m_materials[i].m_pDiffuseTexture)
        {
            aiColor3D color;
            
            if(AI_SUCCESS != pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE,color))
            {
                color.r = 1.0;
                color.g = 0.0;
                color.b = 0.0;
                
                std::cout <<__FUNCTION__<< "set default color red to texture "<<i<<std::endl;
            }
            
             m_materials[i].m_pDiffuseTexture = new Texture(color.r,color.g,color.b);
            
        }
        
        
        
        // TEXTURE_NORMAL
        if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath;
                if(exists(Dir + Path.data))
                    FullPath = Dir +  Path.data;  
                if(exists(Path.data))
                    FullPath = Path.data;
                
                m_materials[i].m_pNormalTexture = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_materials[i].m_pNormalTexture->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_materials[i].m_pNormalTexture;
                    m_materials[i].m_pNormalTexture = NULL;
                    Ret = false;
                }
                else {
                    printf("Loaded Normal texture '%s'\n", FullPath.c_str());
                }
            }
        }
              
        if(!m_materials[i].m_pNormalTexture)
        {
            aiColor3D color;
            
            color.r = 0.5;
            color.g = 0.5;
            color.b = 1.0;
                
            
            
             m_materials[i].m_pNormalTexture = new Texture(color.r,color.g,color.b);
            
        }
        
        
        

            
    }
        
      


        


    return Ret;
}

void Mesh::Render(bool useNormalMap)
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // pos
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)sizeof(glm::vec3)); // uv
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(glm::vec3)+sizeof(glm::vec2))); // normal
        

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        if (MaterialIndex < m_materials.size() && m_materials[MaterialIndex].m_pDiffuseTexture) {
            m_materials[MaterialIndex].m_pDiffuseTexture->Bind(GL_TEXTURE0);
            if(useNormalMap)
                m_materials[MaterialIndex].m_pNormalTexture->Bind(GL_TEXTURE1);
            else
                m_pDefaultNormalMap->Bind(GL_TEXTURE1);
        }

        glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    
}


