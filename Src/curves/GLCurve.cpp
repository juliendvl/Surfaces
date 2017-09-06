#include "curves/GLCurve.h"


GLCurve::GLCurve(const CurvePtr& curve)
    : m_vao(0)
    , m_pvbo(0)
    , m_cvbo(0)
    , m_curve(curve)
    , m_pointSize(5.0f)
    , m_drawControlPoints(true)
    , m_pointColor(1.0f, 0.0f, 0.0f, 1.0f)
    , m_curveColor(1.0f)
{
    GLCHECK(glGenVertexArrays(1, &m_vao));
    if (m_vao == 0)
        Logger::Fatal("Failed to create OpenGL VAO");

    GLCHECK(glGenBuffers(1, &m_pvbo));
    GLCHECK(glGenBuffers(1, &m_cvbo));
    if (m_pvbo == 0 || m_cvbo == 0)
        Logger::Fatal("Failed to create OpenGL VBO");

    GLCHECK(glBindVertexArray(m_vao));

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, m_pvbo));
    GLCHECK(glEnableVertexAttribArray(0));
    GLCHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, m_cvbo));
    GLCHECK(glEnableVertexAttribArray(1));
    GLCHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

    GLCHECK(glBindVertexArray(0));
}

GLCurve::~GLCurve()
{
    if (m_cvbo)
    {
        GLCHECK(glDeleteBuffers(1, &m_cvbo));
        m_cvbo = 0;
    }
    if (m_pvbo)
    {
        GLCHECK(glDeleteBuffers(1, &m_pvbo));
        m_pvbo = 0;
    }
    if (m_vao)
    {
        GLCHECK(glDeleteVertexArrays(1, &m_vao));
        m_vao = 0;
    }
}


void GLCurve::tesselate(size_t nbSamples)
{   
    m_controlPoints = m_curve->getControlPoints();
    
    m_curvePoints.clear();
    float step = 1.0f / (nbSamples - 1);
    for (size_t u = 0; u < nbSamples; ++u)
        m_curvePoints.push_back(m_curve->get_point(u * step));

    // Send data
    GLCHECK(glBindVertexArray(m_vao));

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, m_pvbo));
    GLCHECK(glBufferData(
        GL_ARRAY_BUFFER,
        m_controlPoints.size() * sizeof(glm::vec3),
        m_controlPoints.data(),
        GL_STATIC_DRAW
    ));

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, m_cvbo));
    GLCHECK(glBufferData(
        GL_ARRAY_BUFFER,
        m_curvePoints.size() * sizeof(glm::vec3),
        m_curvePoints.data(),
        GL_STATIC_DRAW
    ));

    GLCHECK(glBindVertexArray(0));
}

void GLCurve::draw(ShaderProgram& pointProgram, ShaderProgram& curveProgram)
{
    GLCHECK(glBindVertexArray(m_vao));

    if (m_drawControlPoints)
    {
        pointProgram.start();
        pointProgram.setUniform("pointSize", m_pointSize);
        pointProgram.setUniform("pointColor", m_pointColor);

        GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, m_pvbo));
        GLCHECK(glDrawArrays(GL_POINTS, 0, m_controlPoints.size()));
    }

    curveProgram.start();
    curveProgram.setUniform("curveColor", m_curveColor);

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, m_cvbo));
    GLCHECK(glDrawArrays(GL_LINE_STRIP, 0, m_curvePoints.size()));

    ShaderProgram::Stop();
    GLCHECK(glBindVertexArray(0));
}