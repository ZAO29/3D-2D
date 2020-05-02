/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FBO.cpp
 * Author: nicolas
 * 
 * Created on October 10, 2019, 9:31 AM
 */

#include "stdafx.h"

#include "FBO.h"

#include "GLGLEW.h"
#include <stdio.h>
#include "Debugging.h"


FBO::FBO() {
}

FBO::FBO(const FBO& orig) {
}

FBO::~FBO() {
     if (m_fbo_ID != 0) {
        glDeleteFramebuffers(1, &m_fbo_ID);
    }

    for (auto& tex : m_texs)
    {
		tex.Destroy();
    }
     
    m_texs.resize(0); 
    
}


bool FBO::Init(unsigned int WindowWidth, unsigned int WindowHeight, unsigned int nbTex)
{
	m_texs.resize(nbTex);
    // Create the FBO
    glGenFramebuffers(1, &m_fbo_ID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_ID); 
    
   
	int toto = 1;
 
   
    
	TexParam paramtex;
	paramtex.m_width = WindowWidth;
	paramtex.m_height = WindowHeight;
	paramtex.m_channel = GL_RGBA;
	paramtex.m_type = GL_FLOAT;

	int i = 0;

    for (auto& tex : m_texs)
    {
		tex.Init(paramtex);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, tex.getID(), 0);

    }
    
    
    // The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
    // Set the list of draw buffers.
    std::vector<GLenum> DrawBuffersAttachement(nbTex);
    for (unsigned int i=0;i < nbTex;i++)
    {
        DrawBuffersAttachement[i] = GL_COLOR_ATTACHMENT0+i;
    }
    
    glDrawBuffers(nbTex, &DrawBuffersAttachement[0]); // "1" is the size of DrawBuffers
    
    
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("ERROR FrameBuffer , status: 0x%x\n", Status);
		INTERNALERROR(" FRAMEBUFFER status not complete");
    }
    
    
    
    // The fullscreen quad's FBO
    static const GLfloat g_quad_vertex_buffer_data[] = { 
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
    };


    glGenBuffers(1, &m_quadMesh_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadMesh_ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    

    return true;
}


void FBO::BindForWriting()
{   
    //GL_FRAMEBUFFER  ou GL_DRAW_FRAMEBUFFER
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_ID);
}

void FBO::BindForReading(unsigned int texUnit, unsigned int texId)
{   
	m_texs[texId].Bind(texUnit);
}



void FBO::getTexData(void * data, unsigned int texId)
{
	m_texs[texId].getData(data);
}

void FBO::RenderQuad()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadMesh_ID);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

    glDisableVertexAttribArray(0);
}



void FBO::BindToScreen()
{
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

