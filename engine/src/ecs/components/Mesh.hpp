#pragma once

#include "../../resources/Geometry.hpp"
#include "../../resources/Material.hpp"
#include "../Component.hpp"
#include <glm/glm.hpp>

class Mesh : public Component {
public:
  Mesh(Geometry *geometry, Material *material)
      : geometry(geometry), material(material) {};
  ~Mesh() = default;

  Geometry *geometry = nullptr;
  Material *material = nullptr;
  static glm::mat4 view;
  static glm::mat4 proj;

  static void SetRenderMatrices(const glm::mat4 &view, const glm::mat4 &proj);

  void Update(float deltaTime) override;
};
