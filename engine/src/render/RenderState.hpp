#pragma once

#include <glm/glm.hpp>

class Light;
class Shader;

class RenderState {
private:
  static glm::mat4 view;
  static glm::mat4 proj;
  static glm::vec3 cameraPosition;
  static glm::vec3 lightPosition;
  static glm::vec3 lightColor;
  static float lightAmbientStrength;
  static float lightDiffuseStrength;
  static float lightSpecularStrength;
  static float lightShininess;

public:
  static void SetMatrices(const glm::mat4 &viewMatrix,
                          const glm::mat4 &projMatrix);
  static void SetCameraPosition(const glm::vec3 &position);
  static void SetLight(const glm::vec3 &position, const Light *light);

  static const glm::mat4 &GetView();
  static const glm::mat4 &GetProj();

  static void ApplyLightingUniforms(const Shader &shader);
};
