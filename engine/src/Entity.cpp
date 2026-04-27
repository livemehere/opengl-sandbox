#include "Entity.hpp"

void Entity::Draw(const glm::mat4 &view, const glm::mat4 &proj) const {
  glm::mat4 model = transform.GetModelMatrix();
  meshRenderer->Draw(model, view, proj);
}
