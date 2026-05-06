#pragma once

#include "../../resources/Geometry.hpp"
#include "../../resources/Material.hpp"
#include "../Component.hpp"

class MeshRenderer : public Component {
public:
  // This component is a renderer that draws a single Geometry + Material combination.
  // In other words, as the name suggests, it is a "rendering component" rather than the "mesh data itself".
  MeshRenderer(Geometry *geometry, Material *material)
      : geometry(geometry), material(material) {};
  ~MeshRenderer() = default;

  Geometry *geometry = nullptr;
  Material *material = nullptr;

  void Update(float deltaTime) override;
};
