#include "Model.hpp"
#include "../Debug.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <stdexcept>

Model::Model(const std::string &path, Shader *shader) : shader(shader) {
  if (!shader) {
    throw std::runtime_error("Model requires a valid shader");
  }

  LoadModel(path);
}

void Model::Draw(const glm::mat4 &modelMatrix, const glm::mat4 &view,
                 const glm::mat4 &proj) const {
  // Model은 내부에 여러 Mesh를 가지고 있으므로
  // draw 요청이 들어오면 각 Mesh를 순회하며 차례대로 그린다.
  for (const auto &mesh : meshes) {
    mesh.Draw(modelMatrix, view, proj);
  }
}

void Model::LoadModel(const std::string &path) {
  Assimp::Importer importer;

  // Triangulate: Ensure all faces are triangles.
  // FlipUVs: Adjust to OpenGL texture coordinate system.
  // GenSmoothNormals: Enable lighting calculations even if the model lacks normals.
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_FlipUVs |
                aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    spdlog::error("Failed to load model '{}': {}", path,
                  importer.GetErrorString());
    throw std::runtime_error("Failed to load model");
  }

  directory = std::filesystem::path(path).parent_path().string();
  ProcessNode(scene->mRootNode, scene);

  spdlog::info("Model loaded successfully: {} (meshes: {})", path,
               meshes.size());
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
  // A single node can have multiple meshes, and child nodes can also have multiple meshes.
  // Therefore, we need to recursively traverse the assimp scene graph to collect all meshes.
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(ProcessMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  vertices.reserve(mesh->mNumVertices);

  // This is the step to convert assimp's aiMesh to the current engine's Vertex format.
  // This project uses Position/Color/TexCoords/Normal, so we fill in that format.
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex{};

    vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                mesh->mVertices[i].z);

    // Model files often lack vertex colors, so the default is set to white.
    // This ensures texture colors or lighting colors appear as originally intended.
    vertex.Color = glm::vec4(1.0f);
    if (mesh->HasVertexColors(0)) {
      vertex.Color = glm::vec4(mesh->mColors[0][i].r, mesh->mColors[0][i].g,
                               mesh->mColors[0][i].b, mesh->mColors[0][i].a);
    }

    if (mesh->HasTextureCoords(0)) {
      vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                   mesh->mTextureCoords[0][i].y);
    } else {
      vertex.TexCoords = glm::vec2(0.0f);
    }

    if (mesh->HasNormals()) {
      vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                mesh->mNormals[i].z);
    } else {
      // We usually don't reach here since we provided the GenSmoothNormals flag,
      // but we initialize with a 0 vector just in case.
      vertex.Normal = glm::vec3(0.0f);
    }

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    const aiFace &face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  Texture *diffuseTexture = nullptr;
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    diffuseTexture = LoadDiffuseTexture(material);
  }

  auto geometry = std::make_unique<Geometry>(vertices, indices);
  auto material = std::make_unique<Material>(shader, diffuseTexture);

  return Mesh(std::move(geometry), std::move(material));
}

Texture *Model::LoadDiffuseTexture(aiMaterial *material) {
  if (!material || material->GetTextureCount(aiTextureType_DIFFUSE) == 0) {
    return nullptr;
  }

  aiString texturePath;
  if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) != AI_SUCCESS) {
    return nullptr;
  }

  const std::string relativePath = texturePath.C_Str();
  const std::string fullPath =
      (std::filesystem::path(directory) / relativePath).lexically_normal().string();

  auto cached = textureCache.find(fullPath);
  if (cached != textureCache.end()) {
    return cached->second;
  }

  try {
    auto texture = std::make_unique<Texture>(fullPath.c_str());
    Texture *texturePtr = texture.get();
    ownedTextures.push_back(std::move(texture));
    textureCache[fullPath] = texturePtr;
    return texturePtr;
  } catch (const std::exception &e) {
    spdlog::warn("Failed to load diffuse texture '{}': {}", fullPath, e.what());
    return nullptr;
  }
}
