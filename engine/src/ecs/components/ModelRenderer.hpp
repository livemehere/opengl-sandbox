#pragma once

#include "../../resources/Model.hpp"
#include "../Component.hpp"

// While the existing MeshRenderer component is designed to draw a single Geometry + Material combination,
// ModelRenderer is a component that draws the entire Model resource.
// In other words, model files containing multiple meshes internally are rendered using this component.
class ModelRenderer : public Component {
public:
  explicit ModelRenderer(Model *model) : model(model) {};
  ~ModelRenderer() = default;

  Model *model = nullptr;

  void Update(float deltaTime) override;
};
