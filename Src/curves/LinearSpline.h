#ifndef LINEAR_SPLINE_H_
#define LINEAR_SPLINE_H_

#include <memory>

#include "Curve.h"


class LinearSpline : public Curve
{
public:
    LinearSpline();
    LinearSpline(const std::vector<glm::vec3>&  points);

    void set_points(const std::vector<glm::vec3>& controlPoints) override;
    glm::vec3 get_point(float param) override;

private:
    void updateParams();
};

using LinearSplinePtr = std::shared_ptr<LinearSpline>;


#endif // LINEAR_SPLINE_H_
