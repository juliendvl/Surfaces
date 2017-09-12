#ifndef HERMITE_SURFACE_H_
#define HERMITE_SURFACE_H_

#include "Surface.h"

#include <GL/glew.h>

#include "curves/GLCurve.h"
#include "curves/HermiteSpline.h"
#include "curves/LinearSpline.h"

enum class InterpolationMode {
  linear = 0,
  hermite_from_polyline = 1,
  hermite_from_ctrl_pts = 2,
  orthogonal_tangent = 3,
};

class HermiteSurface : public Surface
{
  public:

    HermiteSurface();
    HermiteSurface(InterpolationMode time_interpolation_mode);
    HermiteSurface(InterpolationMode time_interpolation_mode, const std::vector<HermiteSplinePtr>& strokes);

    InterpolationMode get_time_interpolation_mode() { return _time_interpolation; };

    void setColor(const glm::vec3& color) { m_color = glm::vec4(color, 1.0f); }
    void setColor(const glm::vec4& color) { m_color = color; }

    /**
     * Draws the key hermite splines.
     */
    void draw() override;

  private:
    InterpolationMode _time_interpolation;
    std::vector<HermiteSplinePtr> _strokes;
    glm::vec3 evaluate(float s, float t) override;

    std::vector<GLCurvePtr> m_keyCurves;
    glm::vec4 m_color;

    void init();
};


#endif // HERMITE_SURFACE_H_
