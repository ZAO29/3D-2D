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

#include "FBO.h"

#include <GL/glew.h>
#include <stdio.h>

FBO::FBO() {
}

FBO::FBO(const FBO& orig) {
}

FBO::~FBO() {
     if (m_fbo_ID != 0) {
        glDeleteFramebuffers(1, &m_fbo_ID);
    }

    for (auto& id : m_tex_IDS)
    {
        glDeleteTextures(1, &id);
    }
     
    m_tex_IDS.resize(0); 
    
}


bool FBO::Init(unsigned int WindowWidth, unsigned int WindowHeight, unsigned int nbTex)
{
    // Create the FBO
    glGenFramebuffers(1, &m_fbo_ID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_ID); 
    
    m_tex_IDS.resize(nbTex);
    
    // Create the depth buffer
    glGenTextures(nbTex, &m_tex_IDS[0]);
    
    for (unsigned int i=0;i<nbTex;i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_tex_IDS[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, m_tex_IDS[i], 0);
    }
    
    
    // The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
    // Set the list of draw buffers.
    std::vector<GLenum> DrawBuffersAttachement;
    DrawBuffersAttachement.resize(nbTex);
    for (unsigned int i=0;i < nbTex;i++)
    {
        DrawBuffersAttachement[i] = GL_COLOR_ATTACHMENT0+i;
    }
    
    glDrawBuffers(nbTex, &DrawBuffersAttachement[0]); // "1" is the size of DrawBuffers
    
    
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("ERROR FrameBuffer , status: 0x%x\n", Status);
        return false;
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
    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_2D, m_tex_IDS[texId]);
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

