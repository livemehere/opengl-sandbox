#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

void Camera::Controls(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    transform.position.x -= 0.1f;
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    transform.position.x += 0.1f;
  } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    transform.position.z -= 0.1f;
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    transform.position.z += 0.1f;
  } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    transform.position.y -= 0.1f;
  } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    transform.position.y += 0.1f;
  }
}

glm::mat4 Camera::GetViewMatrix() const {
  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, -transform.position);
  return view;
}
