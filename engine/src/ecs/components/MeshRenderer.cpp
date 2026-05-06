#include "MeshRenderer.hpp"
#include "../../render/RenderState.hpp"
#include "../Entity.hpp"

void MeshRenderer::Update(float deltaTime) {
  auto transform = owner->GetComponent<Transform>();
  material->Bind(transform->GetModelMatrix(), RenderState::GetView(),
                 RenderState::GetProj());
  RenderState::ApplyLightingUniforms(*material->shader);

  geometry->Bind();
  geometry->Render();
}
