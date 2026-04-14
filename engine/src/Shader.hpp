#pragma once

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

  void SetVec4(const std::string &name, float v1, float v2, float v3,
               float v4) const;
};
