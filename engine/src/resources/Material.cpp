#include "Material.hpp"

void Material::Bind(const glm::mat4 &model, const glm::mat4 &view,
                    const glm::mat4 &proj) const {
  shader->Bind();
  shader->SetMat4("uModel", model);
  shader->SetMat4("uView", view);
  shader->SetMat4("uProj", proj);
  shader->SetBool("uUnlit", unlit);

  if (texture) {
    texture->Bind();
    shader->SetBool("uUseTexture", true);
    shader->SetInt("uTexture", 0);
  } else {
    shader->SetBool("uUseTexture", false);
  }
}
