// OpenGLHelloWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "MyApp.h"

// The MAIN function, from here we start the application and run the game loop
int main()
{
	MyApp app;
	app.Init();
	app.Run();
	app.Destroy();

	
}



