#pragma once

#include "Mesh.hpp"
#include "Texture.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;
class Shader;

// Model is a resource that reads a single file and stores it decomposed into multiple Meshes.
// In other words, it can be seen as a higher-level concept representing the entire obj/fbx/glb file.
class Model {
public:
  Model(const std::string &path, Shader *shader);

  Model(const Model &) = delete;
  Model &operator=(const Model &) = delete;

  void Draw(const glm::mat4 &modelMatrix, const glm::mat4 &view,
            const glm::mat4 &proj) const;

private:
  void LoadModel(const std::string &path);
  void ProcessNode(aiNode *node, const aiScene *scene);
  Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
  Texture *LoadDiffuseTexture(aiMaterial *material);

  // The default shader that all meshes inside the Model will share.
  // If needed, this can be extended to mesh-specific material shaders later.
  Shader *shader = nullptr;

  // Texture paths returned by assimp are usually relative to the model file,
  // so we store the directory where the model file is located.
  std::string directory;

  std::vector<Mesh> meshes;

  // Cache textures by path to avoid uploading the same file to the GPU multiple times.
  std::vector<std::unique_ptr<Texture>> ownedTextures;
  std::unordered_map<std::string, Texture *> textureCache;
};
