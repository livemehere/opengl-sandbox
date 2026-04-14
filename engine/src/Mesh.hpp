#pragma once

#include <vector>

class Mesh {
private:
  unsigned int VAO, VBO, EBO;

public:
  std::vector<unsigned int> indices;
  std::vector<float> vertices;

  Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);
  ~Mesh();

  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  void Bind() const;
  void UnBind() const;
  void Draw() const;
};
