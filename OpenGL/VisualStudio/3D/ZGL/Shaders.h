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
#include <vector>



#define SHADER_MVP "uMVP"

enum eZGLtypeUniform
{
	ZGL_UNDEFINED,
	ZGL_IVEC1,
	ZGL_FVEC1,
	ZGL_FVEC2,
	ZGL_FVEC3,
	ZGL_FVEC4,
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


struct GraphicPipelineType
{
	GraphicPipelineType(){}

	GraphicPipelineType(GraphicPipelineType const &  gpt) :
		vertex(gpt.vertex),
		tesCtrl(gpt.tesCtrl),
		tesEval(gpt.tesEval),
		geometry(gpt.geometry),
		frag(gpt.frag) {}

	bool vertex = true;
	bool tesCtrl = false;
	bool tesEval = false;
	bool geometry = false;
	bool frag = true;
};


class Shader {
public:
    Shader();
    virtual ~Shader();
    
    
    static void setShaderPath(std::string path){s_path = path;}
    
    
    virtual bool Init(std::string name, 
		              MapUniform uniforms  =MapUniform(),
			          GraphicPipelineType type  =GraphicPipelineType(), 
					  std::vector<std::string> subscribedUniform  =std::vector<std::string>());

	void updateUniform(std::string name, const void * pdata);

    void Enable();
    
	static void sCleanHeaderList();

	void Notify(std::string name);

    
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

	// key name of the UniformValueHolder, value boolean true : means value isDirty
	std::map<std::string, bool> m_uniformVarNotification;

	// map of header : 
	//	key filename, 
	// value content
	static std::map<std::string, std::string> s_headersMap;

	static std::string getHeaderContent(std::string includeLine);

};


struct UniformVarValueHolder
{
	static void sAdd(eZGLtypeUniform type, std::string name);
	static void sUpdate(std::string name, const void* pdata);
	static void sSubscribe(std::string name, Shader* psubscribedShader);
	static const void* sgetValue(std::string name);
	static bool sExist(std::string name);
	static eZGLtypeUniform sgetType(std::string name);

	eZGLtypeUniform getType() const { return m_type; }

	const void* getValue() const { return m_pdata; }

	UniformVarValueHolder() {};

private:
	UniformVarValueHolder(eZGLtypeUniform type, std::string name);
	
	void Update(const void* pdata);
	void Subscribe(Shader* psubscribedShader);
	void Destroy();
	


	eZGLtypeUniform m_type = eZGLtypeUniform::ZGL_UNDEFINED;
	void* m_pdata = nullptr;
	std::vector<Shader*> m_subscribedShaders;
	std::string m_name;

	static std::map<std::string, UniformVarValueHolder> s_list;
};

#endif /* SHADERS_H */

