#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <memory>

#include <glm/glm.hpp>


class Surface
{
public:
    Surface() : m_drawInputs(true) {}

    bool drawInputs() const { return m_drawInputs; }
    void setDrawInputs(bool draw) { m_drawInputs = draw; }

    virtual glm::vec3 evaluate(float u, float v) = 0;

    /**
     * If we want to draw specific elements of the surface
     */
    virtual void draw() = 0;

private:
    bool m_drawInputs;
};

using SurfacePtr = std::shared_ptr<Surface>;

#endif // __SURFACE_H__
