#include "surfaces/CoonsPatch.h"

#include <iostream>

#include "viewer/Viewer.h"


CoonsPatch::CoonsPatch(const CurvePtr& C0, const CurvePtr& C1, const CurvePtr& D0, const CurvePtr& D1)
    : Surface()
    , m_C0(C0)
    , m_C1(C1)
    , m_D0(D0)
    , m_D1(D1)
    , m_color(0.0f, 1.0f, 0.0f, 1.0f)
{
    std::vector<CurvePtr> keyCurves = { C0, C1, D0, D1 };
    for (CurvePtr& keyCurve : keyCurves)
    {
        GLCurvePtr curve = std::make_shared<GLCurve>(keyCurve);
        curve->drawControlPoints(false);
        curve->setLineWidth(4.0f);
        curve->setCurveColor(m_color);
        curve->tesselate();

        m_curves.push_back(curve);
    }
}


glm::vec3 CoonsPatch::evaluate(float u, float v)
{
    glm::vec3 Lc = glm::mix(m_C0->get_point(u), m_C1->get_point(u), v);
    glm::vec3 Ld = glm::mix(m_D0->get_point(v), m_D1->get_point(v), u);

    glm::vec3 B = glm::mix(
        glm::mix(m_C0->get_point(0.0f), m_C0->get_point(1.0f), u),
        glm::mix(m_C1->get_point(0.0f), m_C1->get_point(1.0f), u),
        v
    );

    return Lc + Ld - B;
}

void CoonsPatch::draw()
{
    ShaderProgram& pointProgram = *(Viewer::Get().getProgram("point"));
    ShaderProgram& curveProgram = *(Viewer::Get().getProgram("curve"));

    for (GLCurvePtr& keySpline : m_curves)
        keySpline->draw(pointProgram, curveProgram);
}
