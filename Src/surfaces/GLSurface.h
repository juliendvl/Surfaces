#ifndef __GLSURFACE_H__
#define __GLSURFACE_H__

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "surfaces/Surface.h"
#include "viewer/ShaderProgram.h"


class GLSurface
{
public:
    GLSurface(const SurfacePtr& surface);
    ~GLSurface();

    GLSurface(const GLSurface&) = delete;
    GLSurface& operator=(GLSurface&) = delete;

    void setColor(const glm::vec3& color) { m_color = glm::vec4(color, 1.0f); }
    void setColor(const glm::vec4& color) { m_color = color; }

    void tesselate(size_t xStep = 20, size_t yStep = 20);

    void draw(ShaderProgram& program);

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;

    SurfacePtr m_surface;

    std::vector<glm::vec3> m_points;
    std::vector<size_t> m_indices;

    glm::vec4 m_color;
};

using GLSurfacePtr = std::shared_ptr<GLSurface>;

#endif // __GLSURFACE_H__
