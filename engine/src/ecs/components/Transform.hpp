#pragma once

#include "../Component.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform : public Component {
public:
  Transform() = default;
  ~Transform() = default;

  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

  glm::mat4 GetModelMatrix() const;

  void Update(float deltaTime) override {};
};
