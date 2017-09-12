#include <iostream>

#include "surfaces/Grid.h"
#include "surfaces/HermiteSurface.h"
#include "viewer/Viewer.h"


int main()
{
    Logger::SetMinLogLevel(LogLevel::DEBUG_LVL);
    Viewer& viewer = Viewer::Get();

    viewer.addProgram("curve", "../Shaders/curve.vs", "../Shaders/curve.fs");
    viewer.addProgram("point", "../Shaders/point.vs", "../Shaders/point.fs");
    viewer.addProgram("surface", "../Shaders/surface.vs", "../Shaders/surface.fs");
    
    HermiteSurface herm_surf(InterpolationMode::hermite_from_ctrl_pts);
    SurfacePtr surf_ptr = std::make_shared<HermiteSurface>(herm_surf);
    viewer.addSurface(surf_ptr, 20, 20);

    while (viewer.isRunning())
    {
        viewer.handleEvents();
        viewer.draw();
    }

    return 0;
}
