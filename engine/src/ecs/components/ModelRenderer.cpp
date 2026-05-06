#include "../Entity.hpp"
#include "ModelRenderer.hpp"
#include "../../render/RenderState.hpp"

void ModelRenderer::Update(float deltaTime) {
  (void)deltaTime;

  auto *transform = owner ? owner->GetComponent<Transform>() : nullptr;
  if (!transform || !model) {
    return;
  }

  // Use the Entity's Transform as the model matrix for the entire model.
  // All meshes inside the Model share the same position/rotation/scale based on this matrix.
  model->Draw(transform->GetModelMatrix(), RenderState::GetView(),
              RenderState::GetProj());
}
