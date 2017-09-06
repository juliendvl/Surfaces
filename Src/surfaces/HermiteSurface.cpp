
#include "HermiteSurface.h"

HermiteSurface::HermiteSurface()
  : Surface()
{}

HermiteSurface::HermiteSurface(std::string time_interpolation_mode)
  : Surface()
{
  _time_interpolation_mode = time_interpolation_mode;
}

glm::vec3 HermiteSurface::evaluate(float s, float t)
{
  std::vector<glm::vec3> points;
  glm::vec3 result;
  int N = strokes.size();
  for (int i = 0; i < N; ++i) {
    points.push_back(strokes[i].get_point(s));
  }

  if (_time_interpolation_mode == "create_from_ctrl_pts")
  {
    HermiteSpline time_hermite = HermiteSpline(points);
    result = time_hermite.get_point(t);
  }
  else if (_time_interpolation_mode == "linear_interpolation_t")
  {
    LinearSpline linear_time = LinearSpline(points);
    result = linear_time.get_point(t);
  }
  else if (_time_interpolation_mode == "create_from_polyline")
  {
    printf("to do \n");
  }
  else if (_time_interpolation_mode == "t_orthogonal")
  {
    printf("to do \n");
  }
  return result;
}
