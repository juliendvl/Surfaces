
#include "HermiteSurface.h"

HermiteSurface::HermiteSurface()
  : Surface()
{}

HermiteSurface::HermiteSurface(InterpolationMode time_interpolation_mode)
  : Surface()
{
  _time_interpolation = time_interpolation_mode;
}

glm::vec3 HermiteSurface::evaluate(float s, float t)
{
  std::vector<glm::vec3> points;
  glm::vec3 result;
  int N = strokes.size();
  for (int i = 0; i < N; ++i) {
    points.push_back(strokes[i].get_point(s));
  }
  HermiteSpline time_hermite;
  LinearSpline linear_time;
  switch (_time_interpolation)
  {
    case InterpolationMode::hermite_from_ctrl_pts:
      time_hermite = HermiteSpline(points);
      result = time_hermite.get_point(t);
      break;
    case InterpolationMode::linear:
      linear_time = LinearSpline(points);
      result = linear_time.get_point(t);
      break;
    case InterpolationMode::hermite_from_polyline:
      printf("to do\n");
      break;
    case InterpolationMode::orthogonal_tangent:
      printf("to do\n");
      break;
  }
  
  return result;
}
