#ifndef __GLCURVE_H__
#define __GLCURVE_H__

#include <vector>

#include <glm/glm.hpp>

#include "curves/Curve.h"
#include "viewer/ShaderProgram.h"


class GLCurve
{
public:
    GLCurve(const CurvePtr& curve);
    ~GLCurve();

    GLCurve(const GLCurve&) = delete;
    GLCurve& operator=(const GLCurve&) = delete;

    void setPointSize(float size) { m_pointSize = size; }

    void drawControlPoints(bool draw) { m_drawControlPoints = draw; }

    void setPointColor(const glm::vec3& color) { m_pointColor = glm::vec4(color, 1.0f); }
    void setPointColor(const glm::vec4& color) { m_pointColor = color; }

    void setCurveColor(const glm::vec3& color) { m_curveColor = glm::vec4(color, 1.0f); }
    void setCurveColor(const glm::vec4& color) { m_curveColor = color; }

    void tesselate(size_t nbSamples = 50);

    void draw(ShaderProgram& pointProgram, ShaderProgram& curveProgram);

private:
    GLuint m_vao;
    GLuint m_pvbo; // For control points
    GLuint m_cvbo; // For curve

    CurvePtr m_curve;

    std::vector<glm::vec3> m_controlPoints;
    std::vector<glm::vec3> m_curvePoints;

    float m_pointSize;
    bool m_drawControlPoints;

    glm::vec4 m_pointColor;
    glm::vec4 m_curveColor;
};

using GLCurvePtr = std::shared_ptr<GLCurve>;

#endif // __GLCURVE_H__
