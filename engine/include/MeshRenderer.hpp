#pragma once

#include "Material.hpp"
#include "Mesh.hpp"

class MeshRenderer {
public:
  MeshRenderer(Mesh *mesh, Material *material)
      : mesh(mesh), material(material) {};
  ~MeshRenderer() = default;

  Mesh *mesh = nullptr;
  Material *material = nullptr;

  void Draw(const glm::mat4 &model, const glm::mat4 &view,
            const glm::mat4 &proj) const {
    material->Bind(model, view, proj);
    mesh->Bind();
    mesh->Draw();
  }
};
