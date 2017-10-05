#include "DynamicSurface.h"

#include <cmath>
#include <iostream>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "curves/HermiteSpline.h"
#include "curves/LinearSpline.h"
#include "utils/Logger.h"

#define RAD_DEG(rad) rad * 180.0f / 3.141592f
#define DEG_RAD(deg) deg * 3.141592f / 180.0f

static float computeAngle(
    const CurvePtr& C,
    const CurvePtr& prevC,
    const CurvePtr& nextC,
    float s,
    float ds,
    bool derivative
)
{
    if (derivative)
    {
        if (prevC && nextC)
        {
            glm::vec3 vec = prevC->get_point(s + ds) - prevC->get_point(s);
            glm::vec2 vec2D = glm::normalize(glm::vec2(vec.x, vec.y));
            float thetaPrev = atan2f(vec2D.y, vec2D.x);

            vec = nextC->get_point(s + ds) - nextC->get_point(s);
            vec2D = glm::normalize(glm::vec2(vec.x, vec.y));
            float thetaNext = atan2f(vec2D.y, vec2D.x);

            return 0.5f * (thetaNext - thetaPrev);
        }
        else
            return 0.0f;
    }
    else
    {
        glm::vec3 vec = C->get_point(s + ds) - C->get_point(s);

        glm::vec2 vec2D = glm::normalize(glm::vec2(vec.x, vec.y));
        return atan2f(vec2D.y, vec2D.x);
    }
}

static float computeLength(
    const CurvePtr& C,
    const CurvePtr& prevC,
    const CurvePtr& nextC,
    float s,
    float ds,
    bool derivative
)
{
    if (derivative)
    {
        if (prevC && nextC)
        {
            float lengthPrev = glm::length(prevC->get_point(s + ds) - prevC->get_point(s));
            float lengthNext = glm::length(nextC->get_point(s + ds) - nextC->get_point(s));
            
            return 0.5f * (lengthNext - lengthPrev);
        }
        else
            return 0.0f;
    }
    else
        return glm::length(C->get_point(s + ds) - C->get_point(s));
}


DynamicSurface::DynamicSurface(size_t sampling)
    : m_sampling { sampling }
{}

CurvePtr DynamicSurface::interpolate(float t)
{
    // Do we ask for a key stroke ?
    auto it = m_keyStrokes.find(t);
    if (it != m_keyStrokes.end())
        return it->second;

    // Get key strokes surrounding the given time
    auto itlow = std::prev(m_keyStrokes.lower_bound(t));
    auto itup = m_keyStrokes.upper_bound(t);
    if (itlow == m_keyStrokes.end() || itup == m_keyStrokes.end())
    {
        Logger::Error("DynamicSurface::interpolate : invalid time " + std::to_string(t));
        return nullptr;
    }

    CurvePtr& C0 = itlow->second;
    CurvePtr& C1 = itup->second;

    // Get key strokes just before and after key strokes (to compute derivatives)
    CurvePtr& prevC0 = (std::prev(itlow) != m_keyStrokes.end()) ? std::prev(itlow)->second : nullptr;
    CurvePtr& nextC1 = (std::next(itup) != m_keyStrokes.end()) ? std::next(itup)->second : nullptr;

    float normalizedT = (t - itlow->first) / (itup->first - itlow->first);
    return interpolate(C0, C1, prevC0, nextC1, normalizedT);
}


CurvePtr DynamicSurface::interpolate(
    const CurvePtr& C0,
    const CurvePtr& C1,
    const CurvePtr& prevC0,
    const CurvePtr& nextC1,
    float t
)
{
    std::vector<glm::vec3> samples;
    samples.reserve(m_sampling);

    // First interpolate roots
    glm::vec3 R0 = C0->get_point(0.0f);
    glm::vec3 R1 = C1->get_point(0.0f);
    glm::vec3 T0 = prevC0 ? 0.5f * (C1->get_point(0.0f) - prevC0->get_point(0.0f)) : glm::vec3(0.0f);
    glm::vec3 T1 = nextC1 ? 0.5f * (nextC1->get_point(0.0f) - C0->get_point(0.0f)) : glm::vec3(0.0f);

    glm::vec3 prevPoint = Hermite<glm::vec3>(R0, R1, T0, T1, t);
    samples.push_back(prevPoint);

    // Iteratively compute points (angle-length representation)
    float ds = 1.0f / m_sampling;
    std::cout << ds << std::endl;
    for (size_t i = 1; i < m_sampling; ++i)
    {
        float s = i * ds;

        // Angle
        float theta0 = computeAngle(C0, nullptr, nullptr, s, ds, false);
        float theta1 = computeAngle(C1, nullptr, nullptr, s, ds, false);
        
        /*float Dtheta0 = computeAngle(C0, prevC0, C1, s, ds, true);
        float Dtheta1 = computeAngle(C1, C0, nextC1, s, ds, true);
        float angle = Hermite<float>(theta0, theta1, Dtheta0, Dtheta1, t);*/

        float angle = (1.0f - t) * theta0 + t * theta1;

        // Length
        float length0 = computeLength(C0, nullptr, nullptr, s, ds, false);
        float length1 = computeLength(C0, nullptr, nullptr, s, ds, false);
        std::cout << length0 << " " << length1 << std::endl;
        float Dlength0 = computeLength(C0, prevC0, C1, s, ds, true);
        float Dlength1 = computeLength(C1, C0, nextC1, s, ds, true);
        float length = Hermite<float>(length0, length1, Dlength0, Dlength1, t);

        prevPoint += glm::vec3(length * glm::rotate(glm::vec2(1.0f, 0.0f), angle), 0.0f);
        //prevPoint += length * (Q * glm::vec3(1.0f, 0.0f, 0.0f));
        samples.push_back(prevPoint);
    }

    return std::make_shared<LinearSpline>(samples);
}
