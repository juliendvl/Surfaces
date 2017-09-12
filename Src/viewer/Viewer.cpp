#include "viewer/Viewer.h"

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "utils/GLCheck.h"


Viewer Viewer::IViewer;


Viewer& Viewer::Get()
{
    return Viewer::IViewer;
}

void Viewer::setBackgroundColor(const glm::vec3& color)
{
    GLCHECK(glClearColor(color.r, color.g, color.b, 1.0f));
}

void Viewer::addProgram(const std::string& name, const std::string& vFile, const std::string& fFile)
{
    auto program = std::make_shared<ShaderProgram>(vFile, fFile);
    addProgram(name, program);
}

void Viewer::addProgram(const std::string& name, ShaderProgramPtr& program)
{
    m_programs.insert({ name, program });

    program->start();
    program->setUniform("V", m_camera.getViewMatrix());
    program->setUniform("P", m_camera.getProjectionMatrix());
    ShaderProgram::Stop();
}

ShaderProgramPtr Viewer::getProgram(const std::string& name)
{
    auto it = m_programs.find(name);
    return (it != m_programs.end()) ? it->second : nullptr;
}

void Viewer::addCurve(const CurvePtr& curve, size_t nbSamples)
{
    auto glCurve = std::make_shared<GLCurve>(curve);
    glCurve->tesselate(nbSamples);

    m_curves.push_back(glCurve);
}

void Viewer::addCurve(const GLCurvePtr& curve)
{
    m_curves.push_back(curve);
}

void Viewer::addSurface(const SurfacePtr& surface, size_t xStep, size_t yStep)
{
    auto glSurface = std::make_shared<GLSurface>(surface);
    glSurface->tesselate(xStep, yStep);

    m_surfaces.push_back(glSurface);
}

void Viewer::addSurface(const GLSurfacePtr& surface)
{
    m_surfaces.push_back(surface);
}

void Viewer::handleEvents()
{
    sf::Event event;
    m_window.pollEvent(event);

    switch (event.type)
    {
    case sf::Event::Closed:
        m_running = false;
        break;
    case sf::Event::Resized:
        GLCHECK(glViewport(0, 0, event.size.width, event.size.height));
        break;
    case sf::Event::KeyPressed:
        handleKeyEvent(event.key);
        break;
    case sf::Event::MouseButtonPressed:
        handleMousePressEvent(event.mouseButton, true);
        break;
    case sf::Event::MouseButtonReleased:
        handleMousePressEvent(event.mouseButton, false);
        break;
    case sf::Event::MouseMoved:
        handleMouseMoveEvent(event.mouseMove);
        break;
    case sf::Event::MouseWheelMoved:
        handleMouseWheelEvent(event.mouseWheel);
        break;
    default:
        break;
    }
}

void Viewer::draw()
{
    GLCHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    if (m_reloadCamera)
    {
        m_reloadCamera = false;
        for (const auto& it : m_programs)
        {
            it.second->start();
            it.second->setUniform("V", m_camera.getViewMatrix());
            it.second->setUniform("P", m_camera.getProjectionMatrix());
        }
        ShaderProgram::Stop();
    }

    auto curveProgram = m_programs.at("curve");
    auto pointProgram = m_programs.at("point");
    for (const GLCurvePtr& curve : m_curves)
        curve->draw(*pointProgram, *curveProgram);

    auto surfaceProgram = m_programs.at("surface");
    for (const GLSurfacePtr& surface : m_surfaces)
        surface->draw(*surfaceProgram);
    ShaderProgram::Stop();

    m_window.display();
}


Viewer::Viewer()
    : m_window{ sf::VideoMode(1024, 768), 
                "Surfaces", 
                sf::Style::Default, 
                sf::ContextSettings(24, 8, 4, 3, 3, sf::ContextSettings::Attribute::Core) }
    , m_running(true)
    , m_mousePressed(false)
    , m_lastX(0)
    , m_lastY(0)
    , m_camera{glm::vec3(0.0f, 0.0f, 2.0f), 1024.0f / 768.0f}
    , m_reloadCamera(true)
    , m_highlightSurfaces(true)
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        Logger::Fatal("Failed to init GLEW");

    GLCHECK(glEnable(GL_DEPTH_TEST));
    GLCHECK(glDepthFunc(GL_LEQUAL));
    GLCHECK(glEnable(GL_PROGRAM_POINT_SIZE));

    setBackgroundColor(glm::vec3(0.2f));
    GLCHECK(glViewport(0, 0, 1024, 768));
    GLCHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
}

void Viewer::handleKeyEvent(const sf::Event::KeyEvent& keyEvent)
{
    switch (keyEvent.code)
    {
    case sf::Keyboard::H:
        m_highlightSurfaces = !m_highlightSurfaces;
        for (const GLSurfacePtr& surface : m_surfaces)
            surface->useHighlighting(m_highlightSurfaces);
        break;
    case sf::Keyboard::R:
        for (const auto& it : m_programs)
            it.second->compileAndLink();
        break;
    case sf::Keyboard::Left:
        for (const GLSurfacePtr& surface : m_surfaces)
            surface->setHighlightIndex(surface->getHighlightIndex() - 1);
        break;
    case sf::Keyboard::Right:
        for (const GLSurfacePtr& surface : m_surfaces)
            surface->setHighlightIndex(surface->getHighlightIndex() + 1);
        break;
    default:
        break;
    }
}

void Viewer::handleMousePressEvent(const sf::Event::MouseButtonEvent& mouseEvent, bool pressed)
{
    if (pressed && mouseEvent.button == sf::Mouse::Left)
    {
        m_mousePressed = true;
        m_lastX = mouseEvent.x;
        m_lastY = mouseEvent.y;
    }
    else
        m_mousePressed = false;
}

void Viewer::handleMouseMoveEvent(const sf::Event::MouseMoveEvent& mouseEvent)
{
    if (m_mousePressed)
    {
        float xOffset = 0.02f * (mouseEvent.x - m_lastX);
        float yOffset = 0.02f * (m_lastY - mouseEvent.y);

        m_camera.move(xOffset, yOffset);
        m_reloadCamera = true;
    }

    m_lastX = mouseEvent.x;
    m_lastY = mouseEvent.y;
}

void Viewer::handleMouseWheelEvent(const sf::Event::MouseWheelEvent& wheelEvent)
{
    float offset = -0.1f * wheelEvent.delta;
    m_camera.zoom(offset);
    m_reloadCamera = true;
}
