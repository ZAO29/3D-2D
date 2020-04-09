/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Shaders.h
 * Author: nicolas
 *
 * Created on September 18, 2019, 4:12 PM
 */

#ifndef SHADERS_H
#define SHADERS_H


#include <list>
#include <string>
#include <map>





enum eZGLtypeUniform
{
	ZGL_UNDEFINED,
	ZGL_FVEC1,
	ZGL_FVEC2,
	ZGL_FVEC3,
	ZGL_FMAT4,
};



struct UniformVar
{
	unsigned int m_ID = 0;
	eZGLtypeUniform m_type = eZGLtypeUniform::ZGL_UNDEFINED;

	UniformVar(){}

	UniformVar(eZGLtypeUniform type) :m_type(type) {}

	void update(const void * pdata);
};

typedef std::map<std::string, UniformVar> MapUniform;


class Shader {
public:
    Shader();
    virtual ~Shader();
    
    
    static void setShaderPath(std::string path){s_path = path;}
    
    
    virtual bool Init(std::string name, bool  bgeometry, MapUniform uniforms);

	void updateUniform(std::string name, const void * pdata);

    void Enable();
    
 
        

    
protected :
    
    static std::string s_path;
    
    bool LoadShader(const char * file_path);
    
    bool Finalize();
    
    bool SetUniformID(unsigned int& ID,std::string shader_var);

	void initUniforms(MapUniform uniforms);
    
    
private:
    
    // classe mere
    unsigned int m_shaderProg;
    std::list<unsigned int> m_shaderObjList;

	MapUniform m_uniforms;
};

#endif /* SHADERS_H */

