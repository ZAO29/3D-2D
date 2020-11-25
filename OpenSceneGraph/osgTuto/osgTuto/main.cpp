#include <cassert>

#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/ViewerEventHandlers>

#include "ImGuiHandler.hpp"

int
main(int /*argc*/, char* /*argv*/ [])
{

  osgViewer::Viewer viewer;

  auto model = osgDB::readNodeFile("cessna.osg");

  osg::ref_ptr<ImGuiHandler> handler =
    new ImGuiHandler(new ImGuiHandler::GuiCallback);
  handler->setCameraCallbacks(viewer.getCamera());
  viewer.addEventHandler(handler);
  viewer.setSceneData(model);

  return viewer.run();
}
