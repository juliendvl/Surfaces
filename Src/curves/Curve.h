#ifndef __CURVE_H__
#define __CURVE_H__

#include <memory>
#include <vector>

#include <glm/glm.hpp>


class Curve
{
public:
    Curve() {}
    Curve(const std::vector<glm::vec3>& controlPoints)
        : _points(controlPoints)
    {}
    const std::vector<glm::vec3> getControlPoints() const { return _points; }
    virtual void set_points(const std::vector<glm::vec3>& controlPoints) { _points = controlPoints; }

    virtual glm::vec3 get_point(float param) = 0;

protected:
  std::vector<glm::vec3> _points;
  std::vector<float> _params;
  float _length;

private:
  virtual glm::vec3 do_get_point(float param) const { assert(false); return glm::vec3(); }

};

using CurvePtr = std::shared_ptr<Curve>;

#endif // __CURVE_H__
