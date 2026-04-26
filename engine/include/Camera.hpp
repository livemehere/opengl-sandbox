#pragma once

#include "Transform.hpp"
#include <GLFW/glfw3.h>

class Camera {
public:
  glm::mat4 view;
  Transform transform;

  Camera() = default;
  ~Camera() = default;

  void Controls(GLFWwindow *window);
  glm::mat4 GetViewMatrix() const;
};
