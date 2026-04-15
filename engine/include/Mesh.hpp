#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec4 Color;
  glm::vec2 TexCoords;
};

class Mesh {
private:
  unsigned int VAO, VBO, EBO;

public:
  std::vector<unsigned int> indices;
  std::vector<Vertex> vertices;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
  ~Mesh();

  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  void Bind() const;
  void UnBind() const;
  void Draw() const;
};
