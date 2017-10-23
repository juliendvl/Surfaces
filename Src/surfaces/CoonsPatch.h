#ifndef __COONSPATCH_H__
#define __COONSPATCH_H__

#include "surfaces/Surface.h"

#include "curves/GLCurve.h"


class CoonsPatch : public Surface
{
public:
    CoonsPatch(const CurvePtr& C0, const CurvePtr& C1, const CurvePtr& D0, const CurvePtr& D1);

    void setColor(const glm::vec3& color) { m_color = glm::vec4(color, 1.0f); }
    void setColor(const glm::vec4& color) { m_color = color; }

    glm::vec3 evaluate(float u, float v) override;

    void draw() override;

private:
    CurvePtr m_C0;
    CurvePtr m_C1;
    CurvePtr m_D0;
    CurvePtr m_D1;

    std::vector<GLCurvePtr> m_curves;
    glm::vec4 m_color;
};

using CoonsPatchPtr = std::shared_ptr<CoonsPatch>;

#endif // __COONSPATCH_H__
