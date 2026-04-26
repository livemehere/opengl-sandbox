#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

class Entity {
public:
  Transform transform;
  Mesh *mesh = nullptr;
  Texture *texture = nullptr;

  Entity(Mesh *mesh, Texture *texture) : mesh(mesh), texture(texture) {};
  Entity(Mesh *mesh) : mesh(mesh) {};
  ~Entity() = default;

  void Draw(const glm::mat4 &view, const glm::mat4 &proj,
            const Shader &shader) const {
    if (!mesh)
      return;

    shader.Bind();

    if (texture) {
      texture->Bind();
      shader.SetBool("uUseTexture", true);
      shader.SetInt("uTexture", 0);
    }

    auto model = transform.GetModelMatrix();
    shader.SetMat4("uModel", model);

    shader.SetMat4("uModel", model);
    shader.SetMat4("uView", view);
    shader.SetMat4("uProj", proj);

    mesh->Bind();
    mesh->Draw();
  }
};
