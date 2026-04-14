#pragma once

#include <string>

class Shader {
private:
  unsigned int ID;
  void CheckCompileErrors(unsigned int shader, const std::string &type);

public:
  Shader(const char *vertexPath, const char *fragmentPath);
  ~Shader();

  void Bind() const;
  void UnBind() const;
};
