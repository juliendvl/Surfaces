#include "viewer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera()
    : m_position(0.0f, 0.0f, 5.0f)
    , m_aspectRatio(4.0f / 3.0f)
{
    updateViewMatrix();
    updateProjMatrix();
}

Camera::Camera(const glm::vec3& position, float aspectRatio)
    : m_position(position)
    , m_aspectRatio(aspectRatio)
{
    updateViewMatrix();
    updateProjMatrix();
}

void Camera::setAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
    updateProjMatrix();
}

void Camera::move(float xOffset, float yOffset)
{
    m_position += glm::vec3(xOffset, yOffset, 0.0f);
    updateViewMatrix();
}

void Camera::zoom(float offset)
{
    m_position.z += offset;
    if (m_position.z <= 0.2f)
        m_position.z = 0.2f;

    updateViewMatrix();
}


void Camera::updateViewMatrix()
{
    m_viewMatrix = glm::lookAt(
        m_position, 
        glm::vec3(m_position.x, m_position.y, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

void Camera::updateProjMatrix()
{
    m_projMatrix = glm::perspective(45.0f, m_aspectRatio, 0.1f, 1000.0f);
}
