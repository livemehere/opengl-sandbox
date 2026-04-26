#pragma once

#include "Transform.hpp"
#include <GLFW/glfw3.h>

class Camera {
private:
  float speed = 0.1f;
  float pitch = 0.0f;
  float yaw = -90.0f;
  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::normalize(glm::cross(front, up));

public:
  glm::mat4 view;
  glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
  Transform transform;

  Camera() = default;
  ~Camera() = default;

  void Update();
  void Controls(GLFWwindow *window);
  glm::mat4 GetViewMatrix() const;
};
