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

#include "Shaders.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <gtc/type_ptr.hpp>




 std::string Shader::s_path = "glsl/";


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

bool Shader::Init(std::string name, bool  bgeometry)
{
   m_shaderProg = glCreateProgram(); 
   
   if (m_shaderProg == 0) {
        fprintf(stderr, "Error creating shader program\n");
        return false;
    }
 
    if (!LoadShader(std::string(s_path+"/"+name+".vs").c_str())) {
        return false;
    }

    if (bgeometry)
    {
        if(!LoadShader(std::string(s_path+"/"+name+".gs").c_str()))
            return false;
    }
   
    if (!LoadShader(std::string(s_path+"/"+name+".fs").c_str())) {
        return false;
    }
    
    
        
    
    if (!Finalize()) {
        return false;
    }
   
   return true;
}




bool Shader::LoadShader(const char * shader_file_path)
{ 
    if(m_shaderProg == 0)
    {
        std::cout<<"Shader Program not initialized"<<shader_file_path<<std::endl;
        return false;
    }
    
    std::string file(shader_file_path);
    
    std::string ext = file.substr(file.find_last_of(".") + 1);
    
    GLenum shaderType;
    std::string shaderTypeStr= "INVALID";
    std::cout<<__FUNCTION__<<" ext : "<<ext<<std::endl;
    
    if(ext == "vs")
    {
        shaderType = GL_VERTEX_SHADER;
        shaderTypeStr = " VERTEX ";
    }
    if(ext == "fs")
    {
        shaderType = GL_FRAGMENT_SHADER;
        shaderTypeStr = " FRAGMENT ";
    }
    if(ext == "gs")
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
        while(getline(ShaderStream, Line)) 
            ShaderCode += "\n" + Line; 
        ShaderStream.close(); 
    } else
    {
        fprintf(stdout, "ERROR shader : impossible to open %s\n", shader_file_path);
        return false;
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
    glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]); 
    if(!Result)
    {
        fprintf(stdout, "%s\n", &ShaderErrorMessage[0]); 
        return false;
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
        return false;
	}

    glValidateProgram(m_shaderProg);
    glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        return false;
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
        return false;
    }
    
    return true;
    
}


void Shader::Enable()
{
    glUseProgram(m_shaderProg);
}


