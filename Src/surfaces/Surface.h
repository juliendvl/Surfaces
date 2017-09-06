#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <memory>

#include <glm/glm.hpp>


class Surface
{
public:
    virtual glm::vec3 evaluate(float u, float v) = 0;
};

using SurfacePtr = std::shared_ptr<Surface>;

#endif // __SURFACE_H__
