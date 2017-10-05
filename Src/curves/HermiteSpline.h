#ifndef HERMITE_SPLINE_H_
#define HERMITE_SPLINE_H_

#include <memory>

#include "Curve.h"


template<typename T>
T Hermite(const T& P0, const T& P1, const T& T0, const T& T1, float t)
{
    if (t <= 0.0f)
        return P0;
    if (t >= 1.0f)
        return P1;

    float t2 = t * t;
    float t3 = t2 * t;

    // Hermite basis functions
    float h0 = 2.0f * t3 - 3.0f * t2 + 1.0f;
    float h1 = t3 - 2.0f * t2 + t;
    float h2 = -2.0f * t3 + 3.0f * t2;
    float h3 = t3 - t2;

    return h0 * P0 + h1 * T0 + h2 * P1 + h3 * T1;
}


class HermiteSpline : public Curve
{
public:
    HermiteSpline();
    HermiteSpline(const std::vector<glm::vec3>& points);
    HermiteSpline(const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& tangents);
    void set_points(const std::vector<glm::vec3>& points) override ;
    void set_points_tangents(const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& tangents);
    glm::vec3 get_point(float param)  override;

private:
    float compute_length(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& T0, const glm::vec3& T1) const;

    std::vector<glm::vec3> _tangents;
    void update_params();
    void catmull_rom_tangents(float c = 0.5f);
    glm::vec3 hermite_tangent(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& T0, const glm::vec3& T1, float t) const;
};

using HermiteSplinePtr = std::shared_ptr<HermiteSpline>;

#endif // HERMITE_SPLINE_H_
