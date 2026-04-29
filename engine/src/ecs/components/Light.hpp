#pragma once

#include "../Component.hpp"
#include <glm/glm.hpp>

class Light : public Component {
public:
  glm::vec3 color = glm::vec3(1.0f);
  float ambientStrength = 0.2f;
  float diffuseStrength = 1.0f;
  float specularStrength = 0.5f;
  float shininess = 32.0f;
};
