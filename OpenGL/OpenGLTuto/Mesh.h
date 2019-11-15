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

#ifndef MESH_H
#define	MESH_H


#include <GL/glew.h>



#include <map>
#include <vector>
#include <scene.h>       // Output data structure
#include <postprocess.h> // Post processing flags
#include <glm.hpp>
#include"Material.h"
#include <string>



struct Vertex
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;
    glm::vec3 m_tangent;

    Vertex() {}

    Vertex(glm::vec3 pos, glm::vec2 tex)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = glm::vec3(0.);
    }
    
    Vertex(glm::vec3 pos, glm::vec2 tex, glm::vec3 normal)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
    }
};




struct ImportConfig
{
    std::string m_TexFolder = "";
    float m_maxSize = 3.0;
    float m_minSize = 0.5;
};


struct BoundingBox
{
    glm::vec3 _max = glm::vec3(-1.0);
    glm::vec3 _min = glm::vec3(1.0);
};



class Mesh
{
    
    
    
public:
    Mesh();

    ~Mesh();

    bool LoadMesh(const std::string& Filename,const std::string& textureFolder="");

    void Render(bool useNormalMap);
    
    
    ImportConfig m_importConfig;

private:
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMeshsFromNode(aiNode* node, unsigned int &index, const aiScene* pScene, aiMatrix4x4 transform);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh,aiMatrix4x4 transform);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();

    static void GetMeshInfo(aiNode* node, unsigned int& nbMesh,BoundingBox& bbox, const aiScene* pScene, aiMatrix4x4 transform);
    
#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
        MeshEntry();

        ~MeshEntry();

        void Init(const std::vector<Vertex>& Vertices,
                  const std::vector<unsigned int>& Indices);

        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Material> m_materials;
    
    Texture* m_pDefaultNormalMap = new Texture(0.5,0.5,1.0);
    
    float m_scale = 1.0;
    
};


#endif	/* MESH_H */



