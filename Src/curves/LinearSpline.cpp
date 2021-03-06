
#include "LinearSpline.h"

LinearSpline::LinearSpline()
  : Curve()
{}

LinearSpline::LinearSpline(const std::vector<glm::vec3>& points)
  : Curve()
{
  set_points(points);
}

void LinearSpline::set_points(const std::vector<glm::vec3>& controlPoints)
{
  _points = controlPoints;
  updateParams();
}


void LinearSpline::updateParams()
{
  float length = 0.0f;
  for (size_t i = 1; i < _points.size(); ++i)
    length += glm::length(_points[i] - _points[i - 1]);

  _params.clear();
  float dist = 0.0f;

  _params.push_back(0.0f);
  for (size_t i = 1; i < _points.size() - 1; ++i)
  {
    dist += glm::length(_points[i] - _points[i - 1]);
    _params.push_back(dist / length);
  }
  _params.push_back(1.0f);
}

glm::vec3 LinearSpline::get_point(float param)  
{
  if (param <= 0.0f)
    return _points.front();
  if (param >= 1.0f)
    return _points.back();

  for (size_t i = 1; i < _points.size(); ++i)
  {
    if (param < _params[i])
    {
      float t = (param - _params[i - 1]) / (_params[i] - _params[i - 1]);
      return glm::mix(_points[i - 1], _points[i], t);//<glm::vec3, float>
    }
  }

  // Should never be executed !
  assert(false);
  return glm::vec3();
}
