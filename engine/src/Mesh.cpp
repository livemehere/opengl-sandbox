#include "Mesh.hpp"
#include "Debug.hpp"
#include <Engine.hpp>
#include <vector>

Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices) {
  this->vertices = std::move(vertices);
  this->indices = std::move(indices);

  GLCALL(glGenVertexArrays(1, &VAO));
  GLCALL(glGenBuffers(1, &VBO));
  GLCALL(glGenBuffers(1, &EBO));

  GLCALL(glBindVertexArray(VAO));

  // VBO
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  GLCALL(glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float),
                      this->vertices.data(), GL_STATIC_DRAW));

  // EBO
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
  GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      sizeof(unsigned int) * this->indices.size(),
                      this->indices.data(), GL_STATIC_DRAW));

  // layout 0
  GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                               (void *)0));
  GLCALL(glEnableVertexAttribArray(0));
}

Mesh::~Mesh() {
  GLCALL(glDeleteVertexArrays(1, &VAO));
  GLCALL(glDeleteBuffers(1, &VBO));
  GLCALL(glDeleteBuffers(1, &EBO));
}

void Mesh::Bind() const { GLCALL(glBindVertexArray(VAO)); }

void Mesh::UnBind() const { GLCALL(glBindVertexArray(0)); }

void Mesh::Draw() const {
  GLCALL(glDrawElements(GL_TRIANGLES,
                        static_cast<GLsizei>(this->indices.size()),
                        GL_UNSIGNED_INT, nullptr));
}
