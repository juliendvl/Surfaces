#include "surfaces/Grid.h"


Grid::Grid()
    : m_P0(-0.5f, 0.5f, 0.0f)
    , m_P1(0.5f, 0.5f, 0.0f)
    , m_P2(-0.5f, -0.5f, 0.0f)
    , m_P3(0.5f, -0.5f, 0.0f)
{}

Grid::Grid(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3)
    : m_P0(P0)
    , m_P1(P1)
    , m_P2(P2)
    , m_P3(P3)
{}


glm::vec3 Grid::evaluate(float u, float v)
{
    u = glm::clamp(u, 0.0f, 1.0f);
    v = glm::clamp(v, 0.0f, 1.0f);

    glm::vec3 Pv = glm::mix(m_P0, m_P1, v);
    glm::vec3 Qv = glm::mix(m_P2, m_P3, v);

    return glm::mix(Pv, Qv, u);
}
