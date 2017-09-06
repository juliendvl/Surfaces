#ifndef HERMITE_SURFACE_H_
#define HERMITE_SURFACE_H_

#include "Surface.h"
#include "curves\HermiteSpline.h"
#include "curves\LinearSpline.h"

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
    HermiteSurface::HermiteSurface(InterpolationMode time_interpolation_mode, std::vector<HermiteSpline> strokes);
    InterpolationMode get_time_interpolation_mode() { return _time_interpolation; };

  private:
    InterpolationMode _time_interpolation;
    std::vector<HermiteSpline> _strokes;
    glm::vec3 evaluate(float s, float t) override;
};


#endif // HERMITE_SURFACE_H_
