#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Fustrum
{
  float FOV;
  float aspectRatio;
  float nearPlane;
  float farPlane;
};

// The Camera Class

class Camera
{

private:
  glm::mat4 m_view;
  glm::vec3 m_position;
  glm::mat4 m_projection;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;

  float m_yaw = 90.0f;
  float m_pitch = 0.0f;
  float m_speed = 0.1f;

public:
  Fustrum viewFustrum;
  Camera(glm::vec3 position = glm::vec3(0.0f),
         glm::vec3 target = glm::vec3(0.0f, 0.0f, 1.0f),
         glm::vec3 worldUP = glm::vec3(0.0f, 0.0f, 1.0f));

  // Functions to update the camera View matrix
  void update();
  void update(glm::mat4 view);
  void update(glm::vec3 position, glm::vec3 front, glm::vec3 up);

  void updateProjection();

  glm::vec3 getFront() const;
  glm::vec3 getPosition() const;
  glm::vec3 getUp() const;
  glm::vec3 getRight() const;

  glm::mat4 getView() const;
  glm::mat4 getProjection() const;

  void setFront(glm::vec3 front);
  void setPosition(glm::vec3 position);
  void setUp(glm::vec3 up);
  void setRight(glm::vec3 right);
  void setView(glm::mat4 view);
  void setViewFustrum(float FOV, float aspectRatio, float nearPlane, float farPlane);
};