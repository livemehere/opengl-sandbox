#pragma once

#include "Mesh.hpp"
#include "MeshRenderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

class Entity {
public:
  Transform transform;
  MeshRenderer *meshRenderer = nullptr;

  Entity(MeshRenderer *meshRenderer) : meshRenderer(meshRenderer) {};
  ~Entity() = default;

  void Draw(const glm::mat4 &view, const glm::mat4 &proj) const;
};
