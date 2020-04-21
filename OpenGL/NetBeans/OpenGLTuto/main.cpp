#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glx.h>
//#include <SDL2/SDL.h>
#include <ctime>





//perso

#include "Application.h"

#include <iostream>






GLuint VBO;

/*static void CreateVertexBuffer()
{
    Vector3f Vertices[3];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);
 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

}*/







int main(int argc, char* argv[])
{
 
  
  //std::vector<glm::vec3> list_vert = Parser3DModel::OBJParser("//home//nicolas//Documents//NBellot//cube.obj");
  
  Application appli;
  

  appli.Init();
  
  
  while(appli.Run())
  {
      
  }
  
      
  appli.Destroy();    
  

  return 0;
}