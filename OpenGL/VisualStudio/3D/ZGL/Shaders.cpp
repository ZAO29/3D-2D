/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Shaders.cpp
 * Author: nicolas
 * 
 * Created on September 18, 2019, 4:12 PM
 */

#include "stdafx.h"

#include "Shaders.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "GLGLEW.h"
#include "Debugging.h"
#include <glm.hpp>





 std::string Shader::s_path = "";

#define VERTEX_SHADER_EXT "vert"
#define FRAG_SHADER_EXT "frag"
#define GEOM_SHADER_EXT "geom" 
#define TCS_SHADER_EXT "tesc"
#define TES_SHADER_EXT "tese" 



std::map<std::string, std::string> Shader::s_headersMap;

Shader::Shader() : m_shaderProg(0){
}



Shader::~Shader() {
     // Delete the intermediate shader objects that have been added to the program
    // The list will only contain something if shaders were compiled but the object itself
    // was destroyed prior to linking.
    for (auto it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
    {
        glDeleteShader(*it);
    }

    if (m_shaderProg != 0)
    {
        glDeleteProgram(m_shaderProg);
        m_shaderProg = 0;
    }
}

bool Shader::Init(std::string name,
				  MapUniform uniforms,
				  GraphicPipelineType type,
				  std::vector<std::string> subscribedUniforms)
{
   m_shaderProg = glCreateProgram(); 
   
   if (m_shaderProg == 0) {
	   INTERNALERROR(" error creating shader");
    }
 
   if (type.vertex)
   {
	   if (!LoadShader(std::string(name + "." + VERTEX_SHADER_EXT).c_str())) {
		   INTERNALERROR(" error creating vertex shader");
	   }
   }

   if (type.tesCtrl)
   {
	   if (!LoadShader(std::string(name + "." + TCS_SHADER_EXT).c_str())) {
		   INTERNALERROR(" error creating Tesselation Ctrl shader");
	   }
   }

   if (type.tesEval)
   {
	   if (!LoadShader(std::string(name + "." + TES_SHADER_EXT).c_str())) {
		   INTERNALERROR(" error creating Tesselation Eval shader");
	   }
   }


   if (type.geometry)
   {
	   if (!LoadShader(std::string(name + "." + GEOM_SHADER_EXT).c_str()))
	   {
		   INTERNALERROR(" error creating geometry shader");
	   }
    }
   
	if (type.frag)
	{
		if (!LoadShader(std::string(name + "." + FRAG_SHADER_EXT).c_str())) {
			INTERNALERROR(" error creating fragment shader");
		}
	}
    
    
        
    
    if (!Finalize()) {
		INTERNALERROR(" error finalize shader");
    }
   
	//add the subscribed uniform to the map of local uniform
	for (auto& name : subscribedUniforms)
	{
		if (uniforms.find(name) != uniforms.end())
		{
			INTERNALERROR(" error local uniform and global have the same name ");
		}

		if (!UniformVarValueHolder::sExist(name))
		{
			INTERNALERROR(" error local uniform and global have the same name ");
		}

		uniforms[name] = UniformVarValueHolder::sgetType(name);
		
		// shader subscribe to uniformVar
		UniformVarValueHolder::sSubscribe(name, this);

		m_uniformVarNotification[name] = true;
	}

	

	initUniforms(uniforms);

   return true;
}




bool Shader::LoadShader(const char * shader_file_path)
{ 
    if(m_shaderProg == 0)
    {
        std::cout<<"Shader Program not initialized"<<shader_file_path<<std::endl;
		INTERNALERROR("Shader program not initialized");
    }
    
    std::string file(shader_file_path);
    
    std::string ext = file.substr(file.find_last_of(".") + 1);
    
    GLenum shaderType;
    std::string shaderTypeStr= "INVALID";
    std::cout<<__FUNCTION__<<" ext : "<<ext<<std::endl;
    
    if(ext == VERTEX_SHADER_EXT)
    {
        shaderType = GL_VERTEX_SHADER;
        shaderTypeStr = " VERTEX ";
    }

	if (ext == TCS_SHADER_EXT)
	{
		shaderType = GL_TESS_CONTROL_SHADER;
		shaderTypeStr = " TESSELLATION CTRL ";
	}


	if (ext == TES_SHADER_EXT)
	{
		shaderType = GL_TESS_EVALUATION_SHADER;
		shaderTypeStr = " TESSELLATION EVAL ";
	}

    if(ext == FRAG_SHADER_EXT)
    {
        shaderType = GL_FRAGMENT_SHADER;
        shaderTypeStr = " FRAGMENT ";
    }
    if(ext == GEOM_SHADER_EXT)
    {
        shaderType = GL_GEOMETRY_SHADER;
        shaderTypeStr = " GEOMETRY ";
    }
    
    // Crée les shaders 
    GLuint shaderID = glCreateShader(shaderType);
    m_shaderObjList.push_back(shaderID);
   
    // Lit le code du vertex shader à partir du fichier
    std::string ShaderCode; 
    std::ifstream ShaderStream(shader_file_path, std::ios::in); 
    if(ShaderStream.is_open()) 
    { 
        std::string Line = ""; 
		while (getline(ShaderStream, Line))
		{
			if (Line.find("#include") != std::string::npos)
			{
				Line = getHeaderContent(Line);
			}
				
			ShaderCode += "\n" + Line;
		}
        ShaderStream.close(); 
    } else
    {
        fprintf(stdout, "ERROR shader : impossible to open %s\n", shader_file_path);
		INTERNALERROR("impossible to open file");
    }
 
    
 
    GLint Result = GL_FALSE; 
    int InfoLogLength; 
 
    // Compile le vertex shader 
    printf("Compiling shader %s : %s\n", shaderTypeStr.c_str(),shader_file_path); 
    char const * SourcePointer = ShaderCode.c_str(); 
    glShaderSource(shaderID, 1, &SourcePointer , NULL); 
    glCompileShader(shaderID); 
 
    // Vérifie le vertex shader 
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result); 
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength); 
    std::vector<char> ShaderErrorMessage(InfoLogLength);  
    if(!Result)
    {
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &ShaderErrorMessage[0]); 
		INTERNALERROR(" shader error message");
    }
    
    // Lit le programme
    fprintf(stdout, "Linking program\n"); 
    glAttachShader(m_shaderProg, shaderID);
    
    return true;
  
}

bool Shader::Finalize()
{
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(m_shaderProg);

    glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		INTERNALERROR("linking shader");
	}

    glValidateProgram(m_shaderProg);
    glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        INTERNALERROR("invalid error prgm")
    }

    // Delete the intermediate shader objects that have been added to the program
    for (std::list<unsigned int>::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++) {
        glDeleteShader(*it);
    }

    m_shaderObjList.clear();

    return true;
}

bool Shader::SetUniformID(unsigned int& ID,std::string shader_var)
{
    ID = glGetUniformLocation(m_shaderProg, shader_var.c_str());
    
    if(ID == 0xFFFFFFFF)
    {
        std::cout<<__FUNCTION__<< " Invalid location for "<<shader_var<<std::endl;
		INTERNALERROR("error setting uniform ID");
    }
    
    return true;
    
}

void Shader::initUniforms(MapUniform uniforms)
{
	m_uniforms = uniforms;

	for (auto& uniform : m_uniforms)
	{
		SetUniformID(uniform.second.m_ID, uniform.first);
	}
}

void Shader::Notify(std::string name)
{
	if (m_uniformVarNotification.find(name) == m_uniformVarNotification.end())
	{
		INTERNALERROR(" notification of a non present uniformVar");
	}

	m_uniformVarNotification[name] = true;
}

std::string Shader::getHeaderContent(std::string includeLine)
{
	std::size_t firstQuote = includeLine.find("\"");
	std::size_t lastQuote = includeLine.find_last_of("\"");
	if ((firstQuote == std::string::npos) || (lastQuote == std::string::npos))
	{
		fprintf(stdout, "ERROR shader : impossible to open %s\n", includeLine.c_str());
		INTERNALERROR(" shader parsing header impossible to find  quote ");
	}

	std::string preheaderFilename = includeLine.substr(firstQuote+1);
	std::string ext = preheaderFilename.substr(preheaderFilename.find(".")+1);
	std::string name = preheaderFilename.substr(0,preheaderFilename.find("."));
	

	// little trick to avoid the last anti-slash
	if (ext != "header\"")
	{
		INTERNALERROR(" shader parsing header invalid extension ");
	}

	std::string headerFilename = name + ".header";

	// if header already parsed
	if (s_headersMap.find(headerFilename) != s_headersMap.end())
	{
		return s_headersMap[headerFilename];
	}


	std::string headerCode;
	std::ifstream headerStream(headerFilename, std::ios::in);
	if (headerStream.is_open())
	{
		std::string Line = "";
		while (getline(headerStream, Line))
		{
			// WARNING check for circular definition befoe make it recursive
			/*if (Line.find("#include") != std::string::npos)
			{
				Line = getHeaderContent(Line);
			}*/

			headerCode += "\n" + Line;
		}
		headerStream.close();
	}
	else
	{
		fprintf(stdout, "ERROR header shader : impossible to open %s\n", headerFilename.c_str());
		INTERNALERROR("impossible to open file");
	}

	s_headersMap[headerFilename] = headerCode;

	return headerCode;

}

void Shader::updateUniform(std::string name, const void * pdata)
{
	if (m_uniforms.find(name) != m_uniforms.end())
	{
		m_uniforms[name].update(pdata);
	}
	else
	{
		INTERNALERROR(" impossible to update uniform with this name, not present in the map");
	}
}


void Shader::Enable()
{
    glUseProgram(m_shaderProg);

	for (auto& pair : m_uniformVarNotification)
	{
		if (pair.second == true)
		{
			const void* pdata = UniformVarValueHolder::sgetValue(pair.first);
			updateUniform(pair.first, pdata);
			pair.second = false;
		}
	}
}

void Shader::sCleanHeaderList()
{
	if (s_headersMap.size() != 0) s_headersMap.clear(); 
}

void UniformVar::update(const void * pdata)
{
	const GLfloat * pdataf;
	const GLint * pdatai;
	switch (m_type)
	{
		case eZGLtypeUniform::ZGL_UNDEFINED:
			INTERNALERROR("undefined type of uniform variable");
			break;

		case eZGLtypeUniform::ZGL_IVEC1:
			pdatai = reinterpret_cast<const GLint*>(pdata);
			glUniform1iv(m_ID, 1, pdatai);
			break;
		case  eZGLtypeUniform::ZGL_FVEC1:
			pdataf = reinterpret_cast<const GLfloat*>(pdata);
			glUniform1fv(m_ID, 1, pdataf);
			break;
		case  eZGLtypeUniform::ZGL_FVEC2:
			pdataf = reinterpret_cast<const GLfloat*>(pdata);
			glUniform2fv(m_ID, 1, pdataf);
			break;
		case  eZGLtypeUniform::ZGL_FVEC3:
			pdataf = reinterpret_cast<const GLfloat*>(pdata);
			glUniform3fv(m_ID, 1, pdataf);
			break;
		case  eZGLtypeUniform::ZGL_FVEC4:
			pdataf = reinterpret_cast<const GLfloat*>(pdata);
			glUniform4fv(m_ID, 1, pdataf);
			break;

		case eZGLtypeUniform::ZGL_FMAT4 :
			pdataf = reinterpret_cast<const GLfloat*>(pdata);
			glUniformMatrix4fv(m_ID, 1, GL_FALSE, pdataf);
			break;
		
		default :
			INTERNALERROR("unhandle type of uniform variable");
	}
}



std::map<std::string, UniformVarValueHolder> UniformVarValueHolder::s_list;

void UniformVarValueHolder::sAdd(eZGLtypeUniform type, std::string name)
{
	if (s_list.find(name) == s_list.end())
	{
		s_list[name] = UniformVarValueHolder(type, name);
	}
	else
	{
		if (s_list[name].getType() != type)
		{
			INTERNALERROR(" uniformVarValueHolder with same name but with different type already exists");
		}
	}

}

void UniformVarValueHolder::sUpdate(std::string name, const void * pdata)
{
	if (s_list.find(name) != s_list.end())
	{
		s_list[name].Update(pdata);
	}
	else
	{
		INTERNALERROR(" uniformVarValueHolder with this name doesn't exist");
	}
}

void UniformVarValueHolder::sSubscribe(std::string name, Shader * psubscribedShader)
{
	if (s_list.find(name) != s_list.end())
	{
		s_list[name].Subscribe(psubscribedShader);
	}
	else
	{
		INTERNALERROR(" uniformVarValueHolder with this name doesn't exist");
	}
}

const void * UniformVarValueHolder::sgetValue(std::string name)
{
	if (s_list.find(name) != s_list.end())
	{
		return s_list[name].getValue();
	}
	else
	{
		INTERNALERROR(" uniformVarValueHolder with this name doesn't exist");
	}

	return nullptr;
}

bool UniformVarValueHolder::sExist(std::string name)
{
	return (s_list.find(name) != s_list.end());
}

eZGLtypeUniform UniformVarValueHolder::sgetType(std::string name)
{
	return s_list[name].getType();
}

UniformVarValueHolder::UniformVarValueHolder(eZGLtypeUniform type, std::string name):m_type(type),m_name(name)
{
	switch (m_type)
	{
	case eZGLtypeUniform::ZGL_UNDEFINED:
		INTERNALERROR("undefined type of uniform variable");
		break;

	case eZGLtypeUniform::ZGL_IVEC1:
		m_pdata = new int();
		break;

	case  eZGLtypeUniform::ZGL_FVEC1:
		m_pdata = new float();
		break;

	case  eZGLtypeUniform::ZGL_FVEC2:
		m_pdata = new glm::vec2();
		break;

	case  eZGLtypeUniform::ZGL_FVEC3:
		m_pdata = new glm::vec3();
		break;

	case  eZGLtypeUniform::ZGL_FVEC4:
		m_pdata = new glm::vec4();
		break;

	case eZGLtypeUniform::ZGL_FMAT4:
		m_pdata = new glm::mat4();
		break;

	default:
		INTERNALERROR("unhandle type of uniform variable");
	}
}

void UniformVarValueHolder::Update(const void * pdata)
{
	if (m_pdata == nullptr)
	{
		INTERNALERROR(" cannot update UniformVarValueHolder with nullptr data");
	}

	switch (m_type)
	{
	case eZGLtypeUniform::ZGL_UNDEFINED:
		INTERNALERROR("undefined type of uniform variable");
		break;

	case eZGLtypeUniform::ZGL_IVEC1:
		memcpy(m_pdata,pdata,sizeof(int));
		break;

	case  eZGLtypeUniform::ZGL_FVEC1:
		memcpy(m_pdata, pdata, sizeof(float));
		break;

	case  eZGLtypeUniform::ZGL_FVEC2:
		memcpy(m_pdata, pdata, sizeof(glm::vec2));
		break;

	case  eZGLtypeUniform::ZGL_FVEC3:
		memcpy(m_pdata, pdata, sizeof(glm::vec3));
		break;

	case  eZGLtypeUniform::ZGL_FVEC4:
		memcpy(m_pdata, pdata, sizeof(glm::vec4));
		break;

	case eZGLtypeUniform::ZGL_FMAT4:
		memcpy(m_pdata, pdata, sizeof(glm::mat4));
		break;

	default:
		INTERNALERROR("unhandle type of uniform variable");
	}

	for (auto & pshader : m_subscribedShaders)
	{
		pshader->Notify(m_name);
	}
}


void UniformVarValueHolder::Subscribe(Shader * psubscribedShader)
{
	if (psubscribedShader == nullptr)
	{
		INTERNALERROR("a nullptr shader pointer can't subscribed");
	}

	if (std::find(m_subscribedShaders.begin(),m_subscribedShaders.end(),psubscribedShader) == m_subscribedShaders.end())
	{
		m_subscribedShaders.push_back(psubscribedShader);
	}
	else
	{
		INTERNALERROR("shader already subscribed");
	}
}

void UniformVarValueHolder::Destroy()
{
	if (m_pdata = nullptr)
	{
		return;
	}

	switch (m_type)
	{
	case eZGLtypeUniform::ZGL_UNDEFINED:
		INTERNALERROR("undefined type of uniform variable");
		break;

	case eZGLtypeUniform::ZGL_IVEC1:
	{
		int* pdata = reinterpret_cast<int *>(m_pdata);
		delete pdata;
	}
		break;

	case  eZGLtypeUniform::ZGL_FVEC1:
	{
		float* pdata = reinterpret_cast<float *>(m_pdata);
		delete pdata;
	}
		break;

	case  eZGLtypeUniform::ZGL_FVEC2:
	{
		glm::vec2* pdata = reinterpret_cast<glm::vec2*>(m_pdata);
		delete pdata;
	}
		break;

	case  eZGLtypeUniform::ZGL_FVEC3:
	{
		glm::vec3* pdata = reinterpret_cast<glm::vec3*>(m_pdata);
		delete pdata;
	}
		break;

	case  eZGLtypeUniform::ZGL_FVEC4:
	{
		glm::vec4* pdata = reinterpret_cast<glm::vec4*>(m_pdata);
		delete pdata;
	}
		break;

	case eZGLtypeUniform::ZGL_FMAT4:
	{
		glm::mat4* pdata = reinterpret_cast<glm::mat4*>(m_pdata);
		delete pdata;
	}
		break;

	default:
		INTERNALERROR("unhandle type of uniform variable");
	}

	m_pdata = nullptr;
}
