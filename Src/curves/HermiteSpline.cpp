#include "HermiteSpline.h"



HermiteSpline::HermiteSpline()
  : Curve()
{}

HermiteSpline::HermiteSpline(const std::vector<glm::vec3>& points)
  : Curve()
{
  set_points(points);
}

HermiteSpline::HermiteSpline(const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& tangents)
  : Curve()
{
  set_points_tangents(points, tangents);
}


void HermiteSpline::set_points(const std::vector<glm::vec3>& points)
{
  _points = points;

  catmull_rom_tangents();
  update_params();
}

void HermiteSpline::set_points_tangents(const std::vector<glm::vec3>& points,
                                        const std::vector<glm::vec3>& tangents)
{
  assert(points.size() != 0);
  assert(points.size() == tangents.size());
  _points = points;
  _tangents = tangents;

  update_params();
}


float HermiteSpline::compute_length(const glm::vec3& P0, const glm::vec3& P1,
                                    const glm::vec3& T0, const glm::vec3& T1) const
{
  const unsigned int nb_samples = 20;
  float step = 1.0f / nb_samples;
  float length = 0.0f;

  glm::vec3 lastPt = P0;
  for (unsigned int i = 1; i <= nb_samples; ++i)
  {
    glm::vec3 P_t = hermite(P0, P1, T0, T1, i * step);
    length += (P_t - lastPt).length();
    lastPt = P_t;
  }

  return length;
}

void HermiteSpline::update_params()
{
  size_t nb_pts = _points.size();
  _length = 0.0f;

  // For each pair of consecutive points, we generate Hermite interpolated points.
  // The length is then approximated to the length of the generated polyline.
  std::vector<float> lengths;
  for (size_t i = 1; i < nb_pts; ++i)
  {
    if (glm::length(_points[i - 1] - _points[i]) != 0.0f)
      _length += compute_length(_points[i - 1], _points[i], _tangents[i - 1], _tangents[i]);
    lengths.push_back(_length);
  }

  // Update params
  _params.resize(nb_pts);
  _params[0] = 0.0f;
  for (size_t i = 0; i < lengths.size(); ++i)
    _params[i + 1] = (lengths[i] / _length);
}

glm::vec3 HermiteSpline::get_point(float param)  
{
  if (param <= 0.0f)
    return _points.front();
  if (param >= 1.0f)
    return _points.back();

  for (size_t i = 1; i < _params.size(); ++i)
  {
    if (param < _params[i])
    {
      float t = (param - _params[i - 1]) / (_params[i] - _params[i - 1]);
      return hermite(_points[i - 1], _points[i], _tangents[i - 1], _tangents[i], t);
    }
  }

  // Should never be executed !
  assert(false);
  return glm::vec3();
}

void HermiteSpline::catmull_rom_tangents(float c)
{
  _tangents.resize(_points.size());

  // The way we compute tangent is different for first and last points
  _tangents.front() = c * (_points[1] - _points[0]);
  _tangents.back()  = c * (_points.back() - _points[_points.size() - 2]);

  for (size_t i = 1; i < _points.size() - 1; ++i)
    _tangents[i] = c * (_points[i + 1] - _points[i - 1]);
}

glm::vec3 HermiteSpline::hermite(const glm::vec3& P0, const glm::vec3& P1, 
                                  const glm::vec3& T0, const glm::vec3& T1, 
                                  float t) const
{
  float t2 = t * t;
  float t3 = t2 * t;

  // Hermite basis functions
  float h0 = 2.0f * t3 - 3.0f * t2 + 1.0f;
  float h1 = t3 - 2.0f * t2 + t;
  float h2 = -2.0f * t3 + 3.0f * t2;
  float h3 = t3 - t2;

  return h0 * P0 + h1 * T0 + h2 * P1 + h3 * T1;
}

glm::vec3 HermiteSpline::hermite_tangent(const glm::vec3& P0, const glm::vec3& P1,
                                  const glm::vec3& T0, const glm::vec3& T1,
                                  float t) const
{
  float t2 = t * t;
  
  // Hermite derivative values
  float h0 = 6.0f * t * (t - 1.0f);
  float h1 = 3.0f * t2 - 4.0f * t + 1.0f;
  float h2 = 6.0f * t * (-t + 1.0f);
  float h3 = 3.0f * t2 - 2.0f * t;
  
  return glm::normalize(h0 * P0 + h1 * T0 + h2 * P1 + h3 * T1);
}
