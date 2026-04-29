#include "../Entity.hpp"
#include "Mesh.hpp"

glm::mat4 Mesh::view = glm::mat4(1.0f);
glm::mat4 Mesh::proj = glm::mat4(1.0f);
glm::vec3 Mesh::cameraPosition = glm::vec3(0.0f);
glm::vec3 Mesh::lightPosition = glm::vec3(0.0f, 3.0f, 0.0f);
glm::vec3 Mesh::lightColor = glm::vec3(1.0f);
float Mesh::lightAmbientStrength = 0.2f;
float Mesh::lightDiffuseStrength = 1.0f;
float Mesh::lightSpecularStrength = 0.5f;
float Mesh::lightShininess = 32.0f;

void Mesh::SetRenderMatrices(const glm::mat4 &viewMatrix,
                             const glm::mat4 &projMatrix) {
  view = viewMatrix;
  proj = projMatrix;
}

void Mesh::SetCameraPosition(const glm::vec3 &position) {
  cameraPosition = position;
}

void Mesh::SetLight(const glm::vec3 &position, const Light *light) {
  lightPosition = position;

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

void Mesh::Update(float deltaTime) {
  (void)deltaTime;

  auto *transform = owner ? owner->GetComponent<Transform>() : nullptr;
  if (!transform || !geometry || !material) {
    return;
  }

  material->Bind(transform->GetModelMatrix(), view, proj);
  material->shader->SetVec3("uViewPos", cameraPosition.x, cameraPosition.y,
                            cameraPosition.z);
  material->shader->SetVec3("uLight.position", lightPosition.x,
                            lightPosition.y, lightPosition.z);
  material->shader->SetVec3("uLight.color", lightColor.x, lightColor.y,
                            lightColor.z);
  material->shader->SetFloat("uLight.ambientStrength", lightAmbientStrength);
  material->shader->SetFloat("uLight.diffuseStrength", lightDiffuseStrength);
  material->shader->SetFloat("uLight.specularStrength",
                             lightSpecularStrength);
  material->shader->SetFloat("uLight.shininess", lightShininess);
  geometry->Bind();
  geometry->Render();
}
