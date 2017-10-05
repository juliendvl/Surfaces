#include <fstream>
#include <iostream>
#include <sstream>

#include "curves/LinearSpline.h"
#include "surfaces/DynamicSurface.h"
#include "surfaces/HermiteSurface.h"
#include "viewer/Viewer.h"


static void createSurface(DynamicSurface& surface, float t0, float t1, size_t nbStrokes, Viewer& viewer)
{
    float tStep = (t1 - t0) / (nbStrokes - 1);

    for (size_t i = 0; i < nbStrokes; ++i)
    {
        float t = t0 + i * tStep;

        CurvePtr curve = surface.interpolate(t);
        if (curve)
        {
            GLCurvePtr glCurve = std::make_shared<GLCurve>(curve);
            glCurve->drawControlPoints(false);
            glCurve->tesselate();
            viewer.addCurve(glCurve);
        }
    }
}

static void initSplinesFromData(std::vector<CurvePtr>& outSplines)
{
    /*outSplines = {
        std::make_shared<LinearSpline>(std::vector<glm::vec3>{
            glm::vec3(-0.8f, -0.8f, 0.0f),
            glm::vec3(-0.8f, -0.4f, 0.0f),
            glm::vec3(-0.8f,  0.4f, 0.0f),
            glm::vec3(-0.8f,  0.8f, 0.0f)
        }),
        std::make_shared<LinearSpline>(std::vector<glm::vec3>{
            glm::vec3(-0.4f, -0.8f, 0.0f),
            glm::vec3(-0.4f, -0.4f, 0.0f),
            glm::vec3(-0.4f, 0.4f, 0.0f),
            glm::vec3(-0.4f, 0.8f, 0.0f)
        }),
        std::make_shared<LinearSpline>(std::vector<glm::vec3>{
            glm::vec3(0.4f, -0.8f, 0.0f),
            glm::vec3(0.4f, -0.4f, 0.0f),
            glm::vec3(0.4f, 0.4f, 0.0f),
            glm::vec3(0.4f, 0.8f, 0.0f)
        }),
        std::make_shared<LinearSpline>(std::vector<glm::vec3>{
            glm::vec3(0.8f, -0.8f, 0.0f),
            glm::vec3(0.8f, -0.4f, 0.0f),
            glm::vec3(0.8f, 0.4f, 0.0f),
            glm::vec3(0.8f, 0.8f, 0.0f)
        })
    };*/

    outSplines = {
        std::make_shared<HermiteSpline>(
            std::vector<glm::vec3> { glm::vec3(-0.8f, 0.8f, 0.0f), glm::vec3(-0.8f, -0.8f, 0.0f)},
            std::vector<glm::vec3> { glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(2.0f,  -1.0f, 0.0f)}
        ),
        std::make_shared<HermiteSpline>(
            std::vector<glm::vec3> { glm::vec3(0.8f, 0.8f, 0.0f), glm::vec3(0.8f, -0.8f, 0.0f)},
            std::vector<glm::vec3> { glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(-2.0f,  0.0f, 0.0f)}
        )
    };
}

static void initSplineFromFile(const std::string& fileName, std::vector<CurvePtr>& outSplines)
{
    std::ifstream stream(fileName);
    if (stream.is_open())
    {
        std::vector<glm::vec3> samples;

        std::string line;
        while (getline(stream, line))
        {
            std::stringstream ss(line);

            float x, y, z;
            ss >> x >> y >> z;

            samples.push_back(glm::vec3(x, y, z));
        }

        stream.close();
        outSplines.push_back(std::make_shared<LinearSpline>(samples));
    }
}


int main()
{
    Logger::SetMinLogLevel(LogLevel::DEBUG_LVL);
    Viewer& viewer = Viewer::Get();

    viewer.addProgram("curve", "../Shaders/curve.vs", "../Shaders/curve.fs");
    viewer.addProgram("point", "../Shaders/point.vs", "../Shaders/point.fs");
    viewer.addProgram("surface", "../Shaders/surface.vs", "../Shaders/surface.fs");

    std::vector<CurvePtr> splines;
    initSplineFromFile("stroke0.txt", splines);
    initSplineFromFile("stroke1.txt", splines);
    //initSplinesFromData(splines);

    DynamicSurface dynSurface(140);

    float t = 0.0f;
    for (CurvePtr& spline : splines)
    {
        dynSurface.addKeyStroke(t++, spline);

        GLCurvePtr glCurve = std::make_shared<GLCurve>(spline);
        glCurve->drawControlPoints(false);
        glCurve->setLineWidth(4.0f);
        glCurve->setCurveColor(glm::vec3(0.0f, 1.0f, 0.0f));
        glCurve->tesselate();

        viewer.addCurve(glCurve);
    }

    createSurface(dynSurface, 0.0f, --t, 20, viewer);

    while (viewer.isRunning())
    {
        viewer.handleEvents();
        viewer.draw();
    }

    return 0;
}
