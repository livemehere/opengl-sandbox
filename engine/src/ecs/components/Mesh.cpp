#include "../Entity.hpp"
#include "Mesh.hpp"

glm::mat4 Mesh::view = glm::mat4(1.0f);
glm::mat4 Mesh::proj = glm::mat4(1.0f);

void Mesh::SetRenderMatrices(const glm::mat4 &viewMatrix,
                             const glm::mat4 &projMatrix) {
  view = viewMatrix;
  proj = projMatrix;
}

void Mesh::Update(float deltaTime) {
  (void)deltaTime;

  auto *transform = owner ? owner->GetComponent<Transform>() : nullptr;
  if (!transform || !geometry || !material) {
    return;
  }

  material->Bind(transform->GetModelMatrix(), view, proj);
  geometry->Bind();
  geometry->Render();
}
