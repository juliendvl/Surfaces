#include "surfaces/Grid.h"

#include "utils/Logger.h"
#include "viewer/ShaderProgram.h"
#include "viewer/Viewer.h"


Grid::Grid()
    : m_P0(-0.5f, 0.5f, 0.0f)
    , m_P1(0.5f, 0.5f, 0.0f)
    , m_P2(-0.5f, -0.5f, 0.0f)
    , m_P3(0.5f, -0.5f, 0.0f)
    , m_pointSize(10.0f)
    , m_color(0.0f, 1.0f, 0.0f, 1.0f)
{
    init();
}

Grid::Grid(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3)
    : m_P0(P0)
    , m_P1(P1)
    , m_P2(P2)
    , m_P3(P3)
    , m_pointSize(10.0f)
    , m_color(0.0f, 1.0f, 0.0f, 1.0f)
{
    init();
}

Grid::~Grid()
{
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


glm::vec3 Grid::evaluate(float u, float v)
{
    u = glm::clamp(u, 0.0f, 1.0f);
    v = glm::clamp(v, 0.0f, 1.0f);

    glm::vec3 Pv = glm::mix(m_P0, m_P1, v);
    glm::vec3 Qv = glm::mix(m_P2, m_P3, v);

    return glm::mix(Pv, Qv, u);
}

void Grid::draw()
{
    ShaderProgram& program = *(Viewer::Get().getProgram("point"));
    program.start();
    program.setUniform("pointSize", m_pointSize);
    program.setUniform("pointColor", m_color);

    GLCHECK(glBindVertexArray(m_vao));
    GLCHECK(glDrawArrays(GL_POINTS, 0, 4));
    GLCHECK(glBindVertexArray(0));
}


void Grid::init()
{
    GLCHECK(glGenVertexArrays(1, &m_vao));
    if (m_vao == 0)
        Logger::Fatal("Failed to create OpenGL VAO");

    GLCHECK(glGenBuffers(1, &m_vbo));
    if (m_vbo == 0)
        Logger::Fatal("Failed to create OpenGL VBO");

    std::vector<glm::vec3> pts = { m_P0, m_P1, m_P2, m_P3 };

    GLCHECK(glBindVertexArray(m_vao));
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

    GLCHECK(glEnableVertexAttribArray(0));
    GLCHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), pts.data(), GL_STATIC_DRAW));

    GLCHECK(glBindVertexArray(0));
}
