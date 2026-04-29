#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec4 Color;
  glm::vec2 TexCoords;
};

class Geometry {
private:
  unsigned int VAO, VBO, EBO;

public:
  std::vector<unsigned int> indices;
  std::vector<Vertex> vertices;

  Geometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
  ~Geometry();

  Geometry(const Geometry &) = delete;
  Geometry &operator=(const Geometry &) = delete;

  void Bind() const;
  void UnBind() const;
  void Render() const;

  static Geometry CreateCube();
};
