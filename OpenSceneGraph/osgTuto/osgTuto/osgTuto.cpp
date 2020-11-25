// osgTuto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>


int main()
{
	osgViewer::Viewer viewer;
	int numScreen = 1;

	
	

	viewer.setUpViewInWindow(50, 50, 800, 600, numScreen);

	viewer.setSceneData(osgDB::readNodeFile("D:/libraries/osg/repos/OpenSceneGraph-Data/cessna.osg"));
	return viewer.run();
}

