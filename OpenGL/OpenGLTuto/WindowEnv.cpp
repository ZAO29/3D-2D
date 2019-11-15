/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WindowEnv.cpp
 * Author: nicolas
 * 
 * Created on September 18, 2019, 9:33 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <GL/glew.h>



#include "WindowEnv.h"

WindowEnv* WindowEnv::s_pWEnv = NULL; 

WindowEnv::WindowEnv(unsigned int win_w, unsigned int win_h) : 
m_windowWidth(win_w),m_windowHeight(win_h){
}

WindowEnv::WindowEnv(const WindowEnv& orig) {
}

WindowEnv::~WindowEnv() {
}


WindowEnv* WindowEnv::createWindowEnv(unsigned int win_w, unsigned int win_h)
{
    if(s_pWEnv == NULL)
    {
        s_pWEnv = new WindowEnv(win_w,win_h);
    }else
    {
        std::cout<<__FILEFUNC__<<" WindowEnv already exist"<<std::endl;
    }
}


WindowEnv* WindowEnv::deleteWindowEnv()
{
    if(s_pWEnv != NULL)
    {
        delete s_pWEnv;
    }else
    {
        std::cout<<__FILEFUNC__<<" WindowEnv already not created"<<std::endl;
    }
}


void WindowEnv::setWindowSize(unsigned int w, unsigned int h)
{
    m_windowWidth = w;
    m_windowHeight = h;
    
    glViewport(0,0,m_windowWidth,m_windowHeight);
}



bool WindowEnv::ctxErrorOccurred = false;

void WindowEnv::destroy()
{
     glXMakeCurrent( m_pDisplay, 0, 0 );
   glXDestroyContext( m_pDisplay, m_ctx );
  XDestroyWindow( m_pDisplay, m_window );
  XFreeColormap( m_pDisplay, m_cmap );
  XCloseDisplay( m_pDisplay );
}

void WindowEnv::init()
{
    m_pDisplay = XOpenDisplay(NULL);
    
  if (!m_pDisplay)
  {
    printf("Failed to open X m_pDisplay\n");
    exit(1);
  }
    
    
  // Get a matching FB config
  static int visual_attribs[] =
    {
      GLX_X_RENDERABLE    , True,
      GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
      GLX_RENDER_TYPE     , GLX_RGBA_BIT,
      GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
      GLX_RED_SIZE        , 8,
      GLX_GREEN_SIZE      , 8,
      GLX_BLUE_SIZE       , 8,
      GLX_ALPHA_SIZE      , 8,
      GLX_DEPTH_SIZE      , 24,
      GLX_STENCIL_SIZE    , 8,
      GLX_DOUBLEBUFFER    , True,
      GLX_SAMPLE_BUFFERS  , 0,    //MSAA
      GLX_SAMPLES         , 0,    //MSAA
      None
    };

  int glx_major, glx_minor;
 
  // FBConfigs were added in GLX version 1.3.
  if ( !glXQueryVersion( m_pDisplay, &glx_major, &glx_minor ) || 
       ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
  {
    printf("Invalid GLX version");
    exit(1);
  }

  printf( "Getting matching framebuffer configs\n" );
  int fbcount;
  GLXFBConfig* fbc = glXChooseFBConfig(m_pDisplay, DefaultScreen(m_pDisplay), visual_attribs, &fbcount);
  if (!fbc)
  {
    printf( "Failed to retrieve a framebuffer config\n" );
    exit(1);
  }
  printf( "Found %d matching FB configs.\n", fbcount );

  // Pick the FB config/visual with the most samples per pixel
  printf( "Getting XVisualInfos\n" );
  int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

  int i;
  for (i=0; i<fbcount; ++i)
  {
    XVisualInfo *vi = glXGetVisualFromFBConfig( m_pDisplay, fbc[i] );
    if ( vi )
    {
      int samp_buf, samples;
      glXGetFBConfigAttrib( m_pDisplay, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
      glXGetFBConfigAttrib( m_pDisplay, fbc[i], GLX_SAMPLES       , &samples  );
      
      printf( "  Matching fbconfig %d, visual ID 0x%2x: SAMPLE_BUFFERS = %d,"
              " SAMPLES = %d\n", 
              i, vi -> visualid, samp_buf, samples );

      if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
        best_fbc = i, best_num_samp = samples;
      if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
        worst_fbc = i, worst_num_samp = samples;
    }
    XFree( vi );
  }

  GLXFBConfig bestFbc = fbc[ best_fbc ];

  // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
  XFree( fbc );

  // Get a visual
  XVisualInfo *vi = glXGetVisualFromFBConfig( m_pDisplay, bestFbc );
  printf( "Chosen visual ID = 0x%x\n", vi->visualid );

  printf( "Creating colormap\n" );
  XSetWindowAttributes winAttribute;

  winAttribute.colormap = m_cmap = XCreateColormap( m_pDisplay,
                                         RootWindow( m_pDisplay, vi->screen ), 
                                         vi->visual, AllocNone );
  winAttribute.background_pixmap = None ;
  winAttribute.border_pixel      = 0;
  winAttribute.event_mask        = StructureNotifyMask;
  //winAttribute.event_mask = StructureNotifyMask | ExposureMask| KeyPressMask | KeyReleaseMask | ButtonPress | ButtonRelease | PointerMotionMask;
  winAttribute.event_mask = StructureNotifyMask | ExposureMask| KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
  
  printf( "Creating window\n" );
  if(m_windowWidth == 0 || m_windowHeight == 0)
      std::cout<<__FILEFUNC__ << " dimension zero of the window "<<std::endl;
   
  m_window = XCreateWindow( m_pDisplay, RootWindow( m_pDisplay, vi->screen ), 
                              0, 0, m_windowWidth, m_windowHeight, 0, vi->depth, InputOutput, 
                              vi->visual, 
                              CWBorderPixel|CWColormap|CWEventMask, &winAttribute );
  if ( !m_window )
  {
    printf( "Failed to create window.\n" );
    exit(1);
  }

  // Done with the visual info data
  XFree( vi );

  XStoreName( m_pDisplay, m_window, "GL 3.0 Window" );

  printf( "Mapping window\n" );
  XMapWindow( m_pDisplay, m_window );

  // Get the default screen's GLX extension list
  const char *glxExts = glXQueryExtensionsString( m_pDisplay,
                                                  DefaultScreen( m_pDisplay ) );

  // NOTE: It is not necessary to create or make current to a context before
  // calling glXGetProcAddressARB
  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
  glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
           glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );

  m_ctx = 0;

  // Install an X error handler so the application won't exit if GL 3.0
  // context allocation fails.
  //
  // Note this error handler is global.  All display connections in all threads
  // of a process use the same error handler, so be sure to guard against other
  // threads issuing X commands while this code is running.
  ctxErrorOccurred = false;
  int (*oldHandler)(Display*, XErrorEvent*) =
      XSetErrorHandler(&ctxErrorHandler);

  // Check for the GLX_ARB_create_context extension string and the function.
  // If either is not present, use GLX 1.3 context creation method.
  if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) ||
       !glXCreateContextAttribsARB )
  {
    printf( "glXCreateContextAttribsARB() not found"
            " ... using old-style GLX context\n" );
    m_ctx = glXCreateNewContext( m_pDisplay, bestFbc, GLX_RGBA_TYPE, 0, True );
  }

  // If it does, try to get a GL 3.0 context!
  else
  {
    int context_attribs[] =
      {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 0,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
        //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        None
      };

    printf( "Creating context\n" );
    m_ctx = glXCreateContextAttribsARB( m_pDisplay, bestFbc, 0,
                                      True, context_attribs );

    // Sync to ensure any errors generated are processed.
    XSync( m_pDisplay, False );
    if ( !ctxErrorOccurred && m_ctx )
      printf( "Created GL 3.0 context\n" );
    else
    {
      // Couldn't create GL 3.0 context.  Fall back to old-style 2.x context.
      // When a context version below 3.0 is requested, implementations will
      // return the newest context version compatible with OpenGL versions less
      // than version 3.0.
      // GLX_CONTEXT_MAJOR_VERSION_ARB = 1
      context_attribs[1] = 1;
      // GLX_CONTEXT_MINOR_VERSION_ARB = 0
      context_attribs[3] = 0;

      ctxErrorOccurred = false;

      printf( "Failed to create GL 3.0 context"
              " ... using old-style GLX context\n" );
      m_ctx = glXCreateContextAttribsARB( m_pDisplay, bestFbc, 0, 
                                        True, context_attribs );
    }
  }

  // Sync to ensure any errors generated are processed.
  XSync( m_pDisplay, False );

  // Restore the original error handler
  XSetErrorHandler( oldHandler );

  if ( ctxErrorOccurred || !m_ctx )
  {
    printf( "Failed to create an OpenGL context\n" );
    exit(1);
  }

  // Verifying that context is a direct context
  if ( ! glXIsDirect ( m_pDisplay, m_ctx ) )
  {
    printf( "Indirect GLX rendering context obtained\n" );
  }
  else
  {
    printf( "Direct GLX rendering context obtained\n" );
  }

  printf( "Making context current\n" );
  glXMakeCurrent( m_pDisplay, m_window, m_ctx );  
  
  InitGlew();
    
}


void WindowEnv::InitGlew()
{
   glewExperimental = true;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION)); 
}


// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
bool WindowEnv::isExtensionSupported(const char *extList, const char *extension)
{
  const char *start;
  const char *where, *terminator;
  
  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for (start=extList;;) {
    where = strstr(start, extension);

    if (!where)
      break;

    terminator = where + strlen(extension);

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }

  return false;
}

