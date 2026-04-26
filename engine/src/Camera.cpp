#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

void Camera::Controls(GLFWwindow *window) {
  // Movement

  // forward, backward
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    transform.position += front * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    transform.position -= front * speed;
  }

  // left, right
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    transform.position -= right * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    transform.position += right * speed;
  }

  // up, down
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    transform.position -= up * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    transform.position += up * speed;
  }

  // Rotation
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    pitch += 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    pitch -= 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    yaw -= 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    yaw += 1.0f;
  }

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    spdlog::info("Mouse Position: ({}, {})", mouseX, mouseY);
    if (firstClick) {
      lastMouseX = static_cast<float>(mouseX);
      lastMouseY = static_cast<float>(mouseY);
      firstClick = false;
    }

    float xOffset = static_cast<float>(mouseX) - lastMouseX;
    float yOffset = lastMouseY - static_cast<float>(mouseY); // reversed since

    lastMouseX = static_cast<float>(mouseX);
    lastMouseY = static_cast<float>(mouseY);

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;

  } else {
    firstClick = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void Camera::Update() {
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);

  right = glm::normalize(glm::cross(front, up));
}

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(transform.position, transform.position + front, up);
}
