#include "Mesh.hpp"
#include "Debug.hpp"
#include <Engine.hpp>
#include <vector>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
  this->vertices = vertices;
  this->indices = indices;

  GLCALL(glGenVertexArrays(1, &VAO));
  GLCALL(glGenBuffers(1, &VBO));
  GLCALL(glGenBuffers(1, &EBO));

  GLCALL(glBindVertexArray(VAO));

  // VBO
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  GLCALL(glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
                      this->vertices.data(), GL_STATIC_DRAW));

  // EBO
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
  GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      sizeof(unsigned int) * this->indices.size(),
                      this->indices.data(), GL_STATIC_DRAW));

  // layout 0 = Position
  GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                               (void *)offsetof(Vertex, Position)));
  GLCALL(glEnableVertexAttribArray(0));

  // layout 1 = Color
  GLCALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                               (void *)offsetof(Vertex, Color)));
  GLCALL(glEnableVertexAttribArray(1));

  // layout 2 = TexCoords
  GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                               (void *)offsetof(Vertex, TexCoords)));
  GLCALL(glEnableVertexAttribArray(2));

  GLCALL(glBindVertexArray(0));
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

Mesh Mesh::CreateCube() {
  return Mesh(
      {
          // Front face (z = 0.5)
          {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 0
          {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},  // 1
          {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},   // 2
          {{-0.5f, 0.5f, 0.5f},
           {1.0f, 1.0f, 1.0f, 1.0f},
           {0.0f, 1.0f}}, // 3
                          // Back face (z = -0.5)
          {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},  // 4
          {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // 5
          {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},  // 6
          {{0.5f, 0.5f, -0.5f},
           {1.0f, 1.0f, 1.0f, 1.0f},
           {0.0f, 1.0f}}, // 7
                          // Left face (x = -0.5)
          {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 8
          {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},  // 9
          {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},   // 10
          {{-0.5f, 0.5f, -0.5f},
           {1.0f, 1.0f, 1.0f, 1.0f},
           {0.0f, 1.0f}}, // 11
                          // Right face (x = 0.5)
          {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},  // 12
          {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // 13
          {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},  // 14
          {{0.5f, 0.5f, 0.5f},
           {1.0f, 1.0f, 1.0f, 1.0f},
           {0.0f, 1.0f}}, // 15
                          // Top face (y = 0.5)
          {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 16
          {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},  // 17
          {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // 18
          {{-0.5f, 0.5f, -0.5f},
           {1.0f, 1.0f, 1.0f, 1.0f},
           {0.0f, 1.0f}}, // 19
                          // Bottom face (y = -0.5)
          {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 20
          {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},  // 21
          {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},   // 22
          {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}   // 23
      },
      {
          0,  1,  2,  2,  3,  0,  // Front
          4,  5,  6,  6,  7,  4,  // Back
          8,  9,  10, 10, 11, 8,  // Left
          12, 13, 14, 14, 15, 12, // Right
          16, 17, 18, 18, 19, 16, // Top
          20, 21, 22, 22, 23, 20  // Bottom
      });
};
