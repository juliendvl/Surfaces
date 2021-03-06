#include "DynamicSurface.h"

#include <cmath>
#include <iostream>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "curves/HermiteSpline.h"
#include "curves/LinearSpline.h"
#include "utils/Logger.h"


#define RAD_DEG(rad) rad * 180.0f / 3.141592f
#define DEG_RAD(deg) deg * 3.141592f / 180.0f


static const glm::vec3 X_AXIS(1.0f, 0.0f, 0.0f);
static const glm::vec3 Z_AXIS(0.0f, 0.0f, 1.0f);

static float computeAngle(const glm::vec3& source, const glm::vec3& target)
{
    glm::vec3 cross = glm::cross(source, target);

    float angle = atan2f(glm::length(cross), glm::dot(source, target));
    if (glm::dot(cross, Z_AXIS) < 0.0f)
        angle = -angle;

    return angle;
}

static float computeAngle(const CurvePtr& C, float s, float ds)
{
    glm::vec3 vec = C->get_point(s + ds) - C->get_point(s);
    return computeAngle(X_AXIS, glm::normalize(vec));
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
            float prevL = glm::length(prevC->get_point(s + ds) - prevC->get_point(s));
            float nextL = glm::length(nextC->get_point(s + ds) - nextC->get_point(s));
            return 0.5f * (nextL - prevL);
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
    samples.reserve(m_sampling + 1);

    glm::vec3 prevPoint(0.0f);
    samples.push_back(prevPoint);

    // Iteratively compute points (angle-length representation)
    float ds = 1.0f / m_sampling;
    for (size_t i = 0; i < m_sampling; ++i)
    {
        float s = i * ds;

        // Angle
        float A0 = computeAngle(C0, s, ds);
        float A1 = computeAngle(C1, s, ds);

        glm::quat Q0 = glm::angleAxis(A0, Z_AXIS);
        glm::quat Q1 = glm::angleAxis(A1, Z_AXIS);
        glm::quat Q  = glm::slerp(Q0, Q1, t);

        // Length
        float L0  = computeLength(C0, nullptr, nullptr, s, ds, false);
        float L1  = computeLength(C1, nullptr, nullptr, s, ds, false);
        float DL0 = computeLength(nullptr, prevC0, C1, s, ds, true);
        float DL1 = computeLength(nullptr, C0, nextC1, s, ds, true);
        float L   = Hermite<float>(L0, L1, DL0, DL1, t);

        // Compute new point
        prevPoint += L * glm::rotate(Q, X_AXIS);
        samples.push_back(prevPoint);
    }

    // Interpolate roots
    float rootS = 0.0f;
    glm::vec3 R0 = C0->get_point(rootS);
    glm::vec3 R1 = C1->get_point(rootS);
    glm::vec3 T0 = prevC0 ? 0.5f * (C1->get_point(rootS) - prevC0->get_point(rootS)) : glm::vec3(0.0f);
    glm::vec3 T1 = nextC1 ? 0.5f * (nextC1->get_point(rootS) - C0->get_point(rootS)) : glm::vec3(0.0f);
    glm::vec3 R  = Hermite<glm::vec3>(R0, R1, T0, T1, t);

    for (glm::vec3& V : samples)
        V += R;

    return std::make_shared<LinearSpline>(samples);
}
