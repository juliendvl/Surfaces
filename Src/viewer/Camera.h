#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <memory>

#include <glm/glm.hpp>


class Camera
{
public:
    Camera();
    Camera(const glm::vec3& position, float aspectRatio);

    const glm::vec3& getPosition() const { return m_position; }

    float getAspectRatio() const { return m_aspectRatio; }
    void setAspectRatio(float aspectRatio);

    const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
    const glm::mat4& getProjectionMatrix() const { return m_projMatrix; }

    void move(float xOffset, float yOffset);

    void zoom(float offset);

private:
    glm::vec3 m_position;
    float m_aspectRatio;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projMatrix;

    void updateViewMatrix();
    void updateProjMatrix();
};

using CameraPtr = std::shared_ptr<Camera>;

#endif // __CAMERA_H__
