#pragma once

#include "Geometry.hpp"
#include "Material.hpp"
#include <memory>

// A single model file can contain multiple mesh pieces.
// For example, a body, wheels, and windows can each have different vertex buffers/materials.
// That "single piece inside the model" is represented as a Mesh.
class Mesh {
public:
  Mesh(std::unique_ptr<Geometry> geometry, std::unique_ptr<Material> material);

  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;
  Mesh(Mesh &&) noexcept = default;
  Mesh &operator=(Mesh &&) noexcept = default;

  void Draw(const glm::mat4 &modelMatrix, const glm::mat4 &view,
            const glm::mat4 &proj) const;

private:
  // Mesh is responsible for the lifetime of Geometry and Material.
  // This way, simply having the Model own the meshes ensures that all rendering data lifetimes are managed together.
  std::unique_ptr<Geometry> geometry;
  std::unique_ptr<Material> material;
};
