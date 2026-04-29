#pragma once

#include <glm/glm.hpp>
#include <string>

class Shader {
private:
  unsigned int ID;
  void CheckCompileErrors(unsigned int shader, const std::string &type);

public:
  Shader(const char *vertexPath, const char *fragmentPath);
  ~Shader();

  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

  void Bind() const;
  void UnBind() const;

  void SetBool(const std::string &name, bool value) const;
  void SetFloat(const std::string &name, float value) const;
  void SetVec3(const std::string &name, float v1, float v2, float v3) const;
  void SetVec4(const std::string &name, float v1, float v2, float v3,
               float v4) const;
  void SetInt(const std::string &name, int value) const;
  void SetMat4(const std::string &name, const glm::mat4 &model) const;
};
