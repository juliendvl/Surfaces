#ifndef HERMITE_SURFACE_H_
#define HERMITE_SURFACE_H_

#include "Surface.h"
#include "curves\HermiteSpline.h"
#include "curves\LinearSpline.h"

class HermiteSurface : public Surface
{


  public:

    HermiteSurface();
    HermiteSurface(std::string time_interpolation_mode);
    std::string get_time_interpolation_mode() { return _time_interpolation_mode; };

  private:
    std::string _time_interpolation_mode;
    std::vector<HermiteSpline> strokes;
    glm::vec3 evaluate(float s, float t) override;
};
//
//enum {
//  linear_interpolation_t = 0,
//  create_from_polyline = 1,
//  create_from_ctrl_pts = 2,
//  t_orthogonal = 3,
//};

#endif // HERMITE_SURFACE_H_
