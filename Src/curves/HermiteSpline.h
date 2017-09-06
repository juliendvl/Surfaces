#ifndef HERMITE_SPLINE_H_
#define HERMITE_SPLINE_H_

#include "Curve.h"

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
    glm::vec3 hermite(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& T0, const glm::vec3& T1, float t) const;
    glm::vec3 hermite_tangent(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& T0, const glm::vec3& T1, float t) const;

  };

#endif // HERMITE_SPLINE_H_
