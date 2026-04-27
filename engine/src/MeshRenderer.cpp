#include "MeshRenderer.hpp"

void MeshRenderer::Draw(const glm::mat4 &model, const glm::mat4 &view,
                        const glm::mat4 &proj) const {
  material->Bind(model, view, proj);
  mesh->Bind();
  mesh->Draw();
}
