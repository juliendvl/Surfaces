#include "surfaces/GLSurface.h"


GLSurface::GLSurface(const SurfacePtr& surface)
    : m_vao(0)
    , m_vbo(0)
    , m_ibo(0)
    , m_surface(surface)
    , m_color(1.0f)
{
    GLCHECK(glGenVertexArrays(1, &m_vao));
    if (m_vao == 0)
        Logger::Fatal("Failed to create OpenGL VAO");

    GLCHECK(glGenBuffers(1, &m_vbo));
    if (m_vbo == 0)
        Logger::Fatal("Failed to create OpenGL VBO");

    GLCHECK(glGenBuffers(1, &m_ibo));
    if (m_ibo == 0)
        Logger::Fatal("Failed to create OpenGL IBO");

    GLCHECK(glBindVertexArray(m_vao));
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));

    GLCHECK(glEnableVertexAttribArray(0));
    GLCHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

    GLCHECK(glBindVertexArray(0));
}

GLSurface::~GLSurface()
{
    if (m_ibo)
    {
        GLCHECK(glDeleteBuffers(1, &m_ibo));
        m_ibo = 0;
    }
    if (m_vbo)
    {
        GLCHECK(glDeleteBuffers(1, &m_vbo));
        m_vbo = 0;
    }
    if (m_vao)
    {
        GLCHECK(glDeleteVertexArrays(1, &m_vao));
        m_vao = 0;
    }
}


void GLSurface::tesselate(size_t xStep, size_t yStep)
{
    m_points.clear();
    m_indices.clear();

    // Points
    float uStep = 1.0f / (xStep - 1);
    float vStep = 1.0f / (yStep - 1);
    for (size_t v = 0; v < yStep; ++v)
        for (size_t u = 0; u < xStep; ++u)
            m_points.push_back(m_surface->evaluate(u * uStep, v * vStep));

    // Indices
    for (size_t v = 0; v < yStep - 1; ++v)
    {
        for (size_t u = 0; u < xStep - 1; ++u)
        {
            m_indices.push_back(v * xStep + u + 1);
            m_indices.push_back((v + 1) * xStep + u + 1); 
            m_indices.push_back((v + 1) * xStep + u);

            m_indices.push_back(v * xStep + u + 1);
            m_indices.push_back((v + 1) * xStep + u);
            m_indices.push_back(v * xStep + u);
        }
    }

    // Send data
    GLCHECK(glBindVertexArray(m_vao));
    
    GLCHECK(glBufferData(
        GL_ARRAY_BUFFER, 
        m_points.size() * sizeof(glm::vec3), 
        m_points.data(), 
        GL_STATIC_DRAW
    ));
    GLCHECK(glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        m_indices.size() * sizeof(unsigned int), 
        m_indices.data(), 
        GL_STATIC_DRAW
    ));
    
    GLCHECK(glBindVertexArray(0));
}

void GLSurface::draw(ShaderProgram& program)
{
    GLCHECK(glBindVertexArray(m_vao));

    program.setUniform("surfaceColor", m_color);
    GLCHECK(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr));

    GLCHECK(glBindVertexArray(0));
}