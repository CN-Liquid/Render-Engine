#include "Camera.hpp"

// The camera implementation

/*The camera constructor takes in position , target and worldup
It then uses these params to calculate the front and right . Only the camera
position is stored as a member variable */
Camera::Camera(glm::vec3 position, glm::vec3 cameraTarget, glm::vec3 worldUp)
    : m_position(position)
{
  m_front = glm::normalize(cameraTarget - position);
  m_right =
      glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
  m_up = glm::normalize(glm::cross(m_right, m_front));

  // The member variables are used to calculate the view matrix
  m_view = glm::lookAt(m_position, m_position + m_front, m_up);

  // The yaw and pitch are stored to preserve the state of a camera. This
  // ensures no sudden movement when switching cameras
  m_yaw = glm::degrees(atan2(m_front.z, m_front.x));
  m_pitch = glm::degrees(asin(m_front.y));

  setViewFustrum(45.0f, 1.0f, 0.1f, 100.0f);
  updateProjection();
}
void Camera::update(glm::mat4 view) { m_view = view; }

void Camera::update(glm::vec3 position, glm::vec3 front, glm::vec3 up)
{
  m_view = glm::lookAt(position, front + position, up);
}

void Camera::update()
{

  m_right =
      glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
  m_up = glm::normalize(glm::cross(m_right, m_front));

  m_view =
      glm::lookAt(m_position, m_front + m_position, m_up);
}

void Camera::updateProjection()
{
  m_projection = glm::perspective(glm::radians(viewFustrum.FOV), viewFustrum.aspectRatio, viewFustrum.nearPlane, viewFustrum.farPlane);
}

glm::vec3 Camera::getFront() const { return m_front; }
glm::vec3 Camera::getPosition() const { return m_position; }
glm::vec3 Camera::getUp() const { return m_up; }
glm::vec3 Camera::getRight() const { return m_right; }
glm::mat4 Camera::getView() const { return m_view; }
glm::mat4 Camera::getProjection() const { return m_projection; }

void Camera::setFront(glm::vec3 front) { m_front = front; }
void Camera::setPosition(glm::vec3 position) { m_position = position; }
void Camera::setUp(glm::vec3 up) { m_up = up; }
void Camera::setRight(glm::vec3 right) { m_right = right; }

void Camera::setViewFustrum(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
  viewFustrum.FOV = FOV;
  viewFustrum.aspectRatio = aspectRatio;
  viewFustrum.nearPlane = nearPlane;
  viewFustrum.farPlane = farPlane;
}