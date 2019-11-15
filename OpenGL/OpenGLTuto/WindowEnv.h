/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WindowEnv.h
 * Author: nicolas
 *
 * Created on September 18, 2019, 9:33 AM
 */

#ifndef WINDOWENV_H
#define WINDOWENV_H


#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <GL/gl.h>
#include <GL/glx.h>



#include "MyConstant.h"

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092






typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);


// !!!!! SOURCE !!!!!
//https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)


class WindowEnv {
public:
    
    WindowEnv(const WindowEnv& orig);
    
    static WindowEnv* createWindowEnv(unsigned int win_w, unsigned int win_h);
    static WindowEnv* getWindowEnv(){return s_pWEnv;}
    static WindowEnv* deleteWindowEnv();
               
    
    
    virtual ~WindowEnv();
    
    void init();
    
    void swapBuffer()
    {
        glXSwapBuffers ( m_pDisplay, m_window );
    }
    
    void destroy();
    
    static bool isExtensionSupported(const char *extList, const char *extension);
    
    static bool ctxErrorOccurred;
    
    static int ctxErrorHandler( Display *dpy, XErrorEvent *ev )
    {
        ctxErrorOccurred = true;
        return 0;
    }
    
    Display* getDisplay(){return m_pDisplay;}
    Window getWindow(){return m_window;}
    unsigned int getWindowWidth(){return m_windowWidth;};
    unsigned int getWindowHeight(){return m_windowHeight;};
    void setWindowSize(unsigned int w, unsigned int h);
    
    
private:
    WindowEnv(unsigned int win_w, unsigned int win_h);
    void InitGlew();
    
    Display* m_pDisplay = NULL;
    Window m_window;
    GLXContext m_ctx;
    Colormap m_cmap;
    unsigned int m_windowWidth = 0;
    unsigned int m_windowHeight = 0;
    
    
    static WindowEnv* s_pWEnv;
    

};

#endif /* WINDOWENV_H */

