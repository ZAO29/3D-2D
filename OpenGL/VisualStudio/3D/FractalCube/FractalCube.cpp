// FractalCube.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "FractalCubeApp.h"

int main()
{
	FractalCubeApp app;
	app.Init();
	app.Run();
	app.Destroy();
    return 0;
}

