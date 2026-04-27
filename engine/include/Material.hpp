#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

class Material {
public:
  Material(Shader *shader, Texture *texture)
      : shader(shader), texture(texture) {};
  ~Material() = default;

  Shader *shader = nullptr;
  Texture *texture = nullptr;

  void Bind(const glm::mat4 &model, const glm::mat4 &view,
            const glm::mat4 &proj) const {
    shader->Bind();
    shader->SetMat4("uModel", model);
    shader->SetMat4("uView", view);
    shader->SetMat4("uProj", proj);

    // TODO: : inject outside
    shader->SetVec4("uLightColor", 1.0f, 1.0f, 1.0f, 1.0f);

    if (texture) {
      texture->Bind();
      shader->SetBool("uUseTexture", true);
      shader->SetInt("uTexture", 0);
    } else {
      shader->SetBool("uUseTexture", false);
    }
  }
};
