#include "Entity.hpp"
#include "Shader.hpp"

Entity::Entity(Mesh *mesh, Texture *texture) : mesh(mesh), texture(texture) {}
Entity::Entity(Mesh *mesh) : mesh(mesh) {}

void Entity::Draw(const glm::mat4 &view, const glm::mat4 &proj,
                  const Shader &shader) const {
  if (!mesh)
    return;

  shader.Bind();

  shader.SetVec4("uLightColor", 1.0f, 1.0f, 1.0f, 1.0f);

  if (texture) {
    texture->Bind();
    shader.SetBool("uUseTexture", true);
    shader.SetInt("uTexture", 0);
  }

  auto model = transform.GetModelMatrix();
  shader.SetMat4("uModel", model);

  shader.SetMat4("uView", view);
  shader.SetMat4("uProj", proj);

  mesh->Bind();
  mesh->Draw();
}
