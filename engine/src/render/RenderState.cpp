#include "RenderState.hpp"
#include "../ecs/components/Light.hpp"
#include "../resources/Shader.hpp"

glm::mat4 RenderState::view = glm::mat4(1.0f);
glm::mat4 RenderState::proj = glm::mat4(1.0f);
glm::vec3 RenderState::cameraPosition = glm::vec3(0.0f);
glm::vec3 RenderState::lightPosition = glm::vec3(0.0f, 3.0f, 0.0f);
glm::vec3 RenderState::lightColor = glm::vec3(1.0f);
float RenderState::lightAmbientStrength = 0.2f;
float RenderState::lightDiffuseStrength = 1.0f;
float RenderState::lightSpecularStrength = 0.5f;
float RenderState::lightShininess = 32.0f;

void RenderState::SetMatrices(const glm::mat4 &viewMatrix,
                              const glm::mat4 &projMatrix) {
  // The camera's view/projection matrices are commonly used by all renderers.
  view = viewMatrix;
  proj = projMatrix;
}

void RenderState::SetCameraPosition(const glm::vec3 &position) {
  // The camera position is stored separately as it is required for specular calculations.
  cameraPosition = position;
}

void RenderState::SetLight(const glm::vec3 &position, const Light *light) {
  // Use the Transform position of the entity with a Light component as the light source position.
  lightPosition = position;

  // Shader uniforms must always be valid even when there is no Light component in the scene,
  // so we set default values to prevent the rendering pipeline from breaking.
  if (!light) {
    lightColor = glm::vec3(1.0f);
    lightAmbientStrength = 0.2f;
    lightDiffuseStrength = 1.0f;
    lightSpecularStrength = 0.5f;
    lightShininess = 32.0f;
    return;
  }

  lightColor = light->color;
  lightAmbientStrength = light->ambientStrength;
  lightDiffuseStrength = light->diffuseStrength;
  lightSpecularStrength = light->specularStrength;
  lightShininess = light->shininess;
}

const glm::mat4 &RenderState::GetView() { return view; }

const glm::mat4 &RenderState::GetProj() { return proj; }

void RenderState::ApplyLightingUniforms(const Shader &shader) {
  shader.SetVec3("uViewPos", cameraPosition.x, cameraPosition.y,
                 cameraPosition.z);
  shader.SetVec3("uLight.position", lightPosition.x, lightPosition.y,
                 lightPosition.z);
  shader.SetVec3("uLight.color", lightColor.x, lightColor.y, lightColor.z);
  shader.SetFloat("uLight.ambientStrength", lightAmbientStrength);
  shader.SetFloat("uLight.diffuseStrength", lightDiffuseStrength);
  shader.SetFloat("uLight.specularStrength", lightSpecularStrength);
  shader.SetFloat("uLight.shininess", lightShininess);
}
