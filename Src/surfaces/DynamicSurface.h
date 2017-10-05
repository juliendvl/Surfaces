#ifndef __DYNAMIC_SURFACE_H__
#define __DYNAMIC_SURFACE_H__

#include <map>

#include "curves/Curve.h"


class DynamicSurface
{
public:
    DynamicSurface(size_t sampling = 50);

    void addKeyStroke(float t, const CurvePtr& keyStroke) { m_keyStrokes.insert({ t, keyStroke }); }

    size_t getSampling() const { return m_sampling; }
    void setSampling(size_t sampling) { m_sampling = sampling; }

    CurvePtr interpolate(float t);

private:
    std::map<float, CurvePtr> m_keyStrokes;

    size_t m_sampling;

    CurvePtr interpolate(
        const CurvePtr& C0, 
        const CurvePtr& C1, 
        const CurvePtr& prevC0, 
        const CurvePtr& nextC1, 
        float t
    );
};

using DynamicSurfacePtr = std::shared_ptr<DynamicSurface>;

#endif // __DYNAMIC_SURFACE_H__
