#pragma once

#include "../../resources/Geometry.hpp"
#include "../../resources/Material.hpp"
#include "Light.hpp"
#include "../Component.hpp"
#include <glm/glm.hpp>

class Mesh : public Component {
public:
  Mesh(Geometry *geometry, Material *material)
      : geometry(geometry), material(material) {};
  ~Mesh() = default;

  Geometry *geometry = nullptr;
  Material *material = nullptr;
  static glm::mat4 view;
  static glm::mat4 proj;
  static glm::vec3 cameraPosition;
  static glm::vec3 lightPosition;
  static glm::vec3 lightColor;
  static float lightAmbientStrength;
  static float lightDiffuseStrength;
  static float lightSpecularStrength;
  static float lightShininess;

  static void SetRenderMatrices(const glm::mat4 &view, const glm::mat4 &proj);
  static void SetCameraPosition(const glm::vec3 &position);
  static void SetLight(const glm::vec3 &position, const Light *light);

  void Update(float deltaTime) override;
};
