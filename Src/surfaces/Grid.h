#ifndef __GRID_H__
#define __GRID_H__

#include <GL/glew.h>

#include "surfaces/Surface.h"


/**
 * Grid representation : 
 * P0 --------------- P1
 * |                   |
 * |                   |
 * |                   |
 * P2 --------------- P3
 */
class Grid : public Surface
{
public:
    /**
     * The default grid is of size 1.0, centered at origin
     */
    Grid();
    Grid(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3);
    ~Grid();

    void setPointSize(float size) { m_pointSize = size; }
    void setColor(const glm::vec3& color) { m_color = glm::vec4(color, 1.0f); }
    void setColor(const glm::vec4& color) { m_color = color; }

    /**
     * u must be in [0, 1] (otherwise it will be clamped)
     * v must be in [0, 1] (otherwise it will be clamped)
     */
    glm::vec3 evaluate(float u, float v) override;

    /**
     * Draws the four corner points
     */
    void draw() override;

private:
    glm::vec3 m_P0;
    glm::vec3 m_P1;
    glm::vec3 m_P2;
    glm::vec3 m_P3;

    GLuint m_vao, m_vbo;

    float m_pointSize;
    glm::vec4 m_color;

    void init();
};

using GridPtr = std::shared_ptr<Grid>;

#endif // __GRID_H__
