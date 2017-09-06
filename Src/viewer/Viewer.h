#ifndef __VIEWER_H__
#define __VIEWER_H__

#include <unordered_map>

#include <glm/glm.hpp>
#include <SFML/Window.hpp>

#include "surfaces/GLSurface.h"
#include "surfaces/Surface.h"
#include "viewer/Camera.h"


class Viewer
{
public:
    Viewer(const Viewer&) = delete;
    Viewer& operator=(const Viewer&) = delete;

    static Viewer& Get();

    void setBackgroundColor(const glm::vec3& color);

    void addProgram(const std::string& name, const std::string& vFile, const std::string& fFile);
    void addProgram(const std::string& name, ShaderProgramPtr& program);

    void addSurface(const SurfacePtr& surface, size_t xStep = 10, size_t yStep = 10);
    void addSurface(const GLSurfacePtr& surface);

    bool isRunning() const { return m_running; }
    void handleEvents();
    void draw();

private:
    static Viewer IViewer;

    sf::Window m_window;
    bool m_running;

    bool m_mousePressed;
    int m_lastX, m_lastY;

    Camera m_camera;
    bool m_reloadCamera;

    std::unordered_map<std::string, ShaderProgramPtr> m_programs;

    std::vector<GLSurfacePtr> m_surfaces;

    Viewer();

    void handleKeyEvent(const sf::Event::KeyEvent& keyEvent);
    void handleMousePressEvent(const sf::Event::MouseButtonEvent& mouseEvent, bool pressed);
    void handleMouseMoveEvent(const sf::Event::MouseMoveEvent& mouseEvent);
    void handleMouseWheelEvent(const sf::Event::MouseWheelEvent& wheelEvent);
};

#endif // __VIEWER_H__
