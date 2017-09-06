#include <iostream>

#include "surfaces/Grid.h"
#include "viewer/Viewer.h"


int main()
{
    Logger::SetMinLogLevel(LogLevel::DEBUG_LVL);
    Viewer& viewer = Viewer::Get();

    viewer.addProgram("surface", "../Shaders/surface.vs", "../Shaders/surface.fs");

    viewer.addSurface(std::make_shared<Grid>(
        glm::vec3(-0.8f, 0.8f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.8f, -0.8f, 0.0f)
    ));

    while (viewer.isRunning())
    {
        viewer.handleEvents();
        viewer.draw();
    }

    return 0;
}
