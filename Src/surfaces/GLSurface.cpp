#include "surfaces/GLSurface.h"


GLSurface::GLSurface(const SurfacePtr& surface)
    : m_vao(0)
    , m_vbo(0)
    , m_ibo(0)
    , m_surface(surface)
    , m_xStep(0)
    , m_yStep(0)
    , m_color(1.0f)
    , m_highlight(true)
    , m_highlightIndex(0)
    , m_highlightColor(1.0f, 0.0f, 0.0f, 1.0f)
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

    m_points.reserve(xStep * yStep);
    m_indices.reserve(2 * xStep * yStep);

    m_xStep = xStep;
    m_yStep = yStep;

    // Points
    float uStep = 1.0f / (xStep - 1);
    float vStep = 1.0f / (yStep - 1);
    for (size_t v = 0; v < yStep; ++v)
        for (size_t u = 0; u < xStep; ++u)
            m_points.push_back(m_surface->evaluate(u * uStep, v * vStep));

    // Indices of columns
    for (unsigned int i = 0; i < (unsigned int)xStep * yStep; ++i)
        m_indices.push_back(i);

    // Indices of rows
    for (unsigned int u = 0; u < (unsigned int)xStep; ++u)
        for (unsigned int v = 0; v < (unsigned int)yStep; ++v)
            m_indices.push_back(u + v * xStep);

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

    // Draw columns
    for (size_t v = 0; v < m_yStep; ++v)
    {
        if (m_highlight && v == m_highlightIndex)
        {
            GLCHECK(glLineWidth(4.0f));
            program.setUniform("surfaceColor", m_highlightColor);
        }

        GLCHECK(glDrawElements(
            GL_LINE_STRIP,
            m_xStep,
            GL_UNSIGNED_INT,
            (void*)(v * m_xStep * sizeof(unsigned int))
        ));

        if (m_highlight && v == m_highlightIndex)
        {
            GLCHECK(glLineWidth(1.0f));
            program.setUniform("surfaceColor", m_color);
        }
    }

    // Draw lines
    for (size_t u = 0; u < m_xStep; ++u)
        GLCHECK(glDrawElements(
            GL_LINE_STRIP,
            m_yStep,
            GL_UNSIGNED_INT,
            (void*)((m_xStep * m_yStep + u * m_yStep) * sizeof(unsigned int))
        ));

    GLCHECK(glBindVertexArray(0));
}