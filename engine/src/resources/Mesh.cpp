#include "Mesh.hpp"
#include "../render/RenderState.hpp"

Mesh::Mesh(std::unique_ptr<Geometry> geometry,
           std::unique_ptr<Material> material)
    : geometry(std::move(geometry)), material(std::move(material)) {}

void Mesh::Draw(const glm::mat4 &modelMatrix, const glm::mat4 &view,
                const glm::mat4 &proj) const {
  if (!geometry || !material || !material->shader) {
    return;
  }

  // Since each Mesh can have different Material/Texture,
  // we bind its own material first at every draw call.
  material->Bind(modelMatrix, view, proj);

  // Lighting uniforms are provided based on the common render state of the current frame.
  // This allows single MeshRenderer and internal meshes of Model to share the same lighting system.
  RenderState::ApplyLightingUniforms(*material->shader);

  geometry->Bind();
  geometry->Render();
}
