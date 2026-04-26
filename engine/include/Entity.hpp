#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

class Entity {
public:
  Transform transform;
  Mesh *mesh = nullptr;
  Texture *texture = nullptr;

  Entity(Mesh *mesh, Texture *texture);
  Entity(Mesh *mesh);
  ~Entity() = default;

  void Draw(const glm::mat4 &view, const glm::mat4 &proj,
            const Shader &shader) const;
};
