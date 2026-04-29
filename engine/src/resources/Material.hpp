#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

class Material {
public:
  Material(Shader *shader, Texture *texture)
      : shader(shader), texture(texture) {};
  Material(Shader *shader) : shader(shader) {};

  ~Material() = default;

  Shader *shader = nullptr;
  Texture *texture = nullptr;

  void Bind(const glm::mat4 &model, const glm::mat4 &view,
            const glm::mat4 &proj) const;
};
