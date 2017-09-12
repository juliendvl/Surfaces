#include "HermiteSurface.h"

#include "viewer/Viewer.h"


HermiteSurface::HermiteSurface()
    : Surface()
    , m_color(0.0f, 1.0f, 0.0f, 1.0f)
{
  _time_interpolation = InterpolationMode::linear;

  //create strokes by default straight and parallels
  std::vector<glm::vec3> points;
  points.push_back(glm::vec3(-0.4f, 0.2f, 0.0f));
  points.push_back(glm::vec3(-0.4f, 0.4f, 0.0f));
  points.push_back(glm::vec3(-0.4f, 0.6f, 0.0f));
  points.push_back(glm::vec3(-0.4f, 0.8f, 0.0f));
  HermiteSplinePtr stroke_1 = std::make_shared<HermiteSpline>(points);

  points.clear();
  points.push_back(glm::vec3(-0.2f, 0.2f, 0.0f));
  points.push_back(glm::vec3(-0.2f, 0.4f, 0.0f));
  points.push_back(glm::vec3(-0.2f, 0.6f, 0.0f));
  points.push_back(glm::vec3(-0.2f, 0.8f, 0.0f));
  HermiteSplinePtr stroke_2 = std::make_shared<HermiteSpline>(points);


  points.clear();
  points.push_back(glm::vec3(0.2f, 0.2f, 0.0f));
  points.push_back(glm::vec3(0.2f, 0.4f, 0.0f));
  points.push_back(glm::vec3(0.2f, 0.6f, 0.0f));
  points.push_back(glm::vec3(0.2f, 0.8f, 0.0f));
  HermiteSplinePtr stroke_3 = std::make_shared<HermiteSpline>(points);

  points.clear();
  points.push_back(glm::vec3(0.4f, 0.2f, 0.0f));
  points.push_back(glm::vec3(0.4f, 0.4f, 0.0f));
  points.push_back(glm::vec3(0.4f, 0.6f, 0.0f));
  points.push_back(glm::vec3(0.4f, 0.8f, 0.0f));
  HermiteSplinePtr stroke_4 = std::make_shared<HermiteSpline>(points);

  _strokes.push_back(stroke_1);
  _strokes.push_back(stroke_2);
  _strokes.push_back(stroke_3);
  _strokes.push_back(stroke_4);

  init();
}

HermiteSurface::HermiteSurface(InterpolationMode time_interpolation_mode)
    : Surface()
    , m_color(0.0f, 1.0f, 0.0f, 1.0f)
{
  _time_interpolation = time_interpolation_mode;
  
  //create strokes by default
  std::vector<glm::vec3> points;
  points.push_back(glm::vec3(-0.4f, 0.2f, 0.0f));
  points.push_back(glm::vec3(-0.4f, 0.4f, 0.0f));
  points.push_back(glm::vec3(-0.4f, 0.8f, 0.0f));
  points.push_back(glm::vec3(-0.4f, 0.99f, 0.0f));
  HermiteSplinePtr stroke_1 = std::make_shared<HermiteSpline>(points);

  points.clear();
  points.push_back(glm::vec3(-0.2f, 0.3f, 0.0f));
  points.push_back(glm::vec3(-0.2f, 0.5f, 0.0f));
  points.push_back(glm::vec3(-0.2f, 0.6f, 0.0f));
  points.push_back(glm::vec3(-0.2f, 0.7f, 0.0f));
  HermiteSplinePtr stroke_2 = std::make_shared<HermiteSpline>(points);


  points.clear();
  points.push_back(glm::vec3(0.2f, 0.1f, 0.0f));
  points.push_back(glm::vec3(0.2f, 0.3f, 0.0f));
  points.push_back(glm::vec3(0.2f, 0.5f, 0.0f));
  points.push_back(glm::vec3(0.2f, 0.7f, 0.0f));
  HermiteSplinePtr stroke_3 = std::make_shared<HermiteSpline>(points);

  points.clear();
  points.push_back(glm::vec3(0.4f, 0.2f, 0.0f));
  points.push_back(glm::vec3(0.4f, 0.4f, 0.0f));
  points.push_back(glm::vec3(0.4f, 0.6f, 0.0f));
  points.push_back(glm::vec3(0.4f, 0.8f, 0.0f));
  HermiteSplinePtr stroke_4 = std::make_shared<HermiteSpline>(points);

  _strokes.push_back(stroke_1);
  _strokes.push_back(stroke_2);
  _strokes.push_back(stroke_3);
  _strokes.push_back(stroke_4);

  init();
}

HermiteSurface::HermiteSurface(InterpolationMode time_interpolation_mode, const std::vector<HermiteSplinePtr>& strokes)
    : Surface()
    , _time_interpolation(time_interpolation_mode)
    , _strokes(strokes)
    , m_color(0.0f, 1.0f, 0.0f, 1.0f)
{
    init();
}


void HermiteSurface::draw()
{
    ShaderProgram& pointProgram = *(Viewer::Get().getProgram("point"));
    ShaderProgram& curveProgram = *(Viewer::Get().getProgram("curve"));

    GLCHECK(glLineWidth(4.0f));
    for (GLCurvePtr& keySpline : m_keyCurves)
        keySpline->draw(pointProgram, curveProgram);
    GLCHECK(glLineWidth(1.0f));
}


glm::vec3 HermiteSurface::evaluate(float s, float t)
{
  std::vector<glm::vec3> points;
  glm::vec3 result;
  int N = _strokes.size();
  for (int i = 0; i < N; ++i) {
    points.push_back(_strokes[i]->get_point(s));
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

void HermiteSurface::init()
{
    for (HermiteSplinePtr& keySpline : _strokes)
    {
        GLCurvePtr curve = std::make_shared<GLCurve>(keySpline);
        curve->drawControlPoints(false);
        curve->setCurveColor(m_color);
        curve->tesselate();

        m_keyCurves.push_back(curve);
    }
}
