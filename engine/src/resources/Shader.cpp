#include "../Debug.hpp"
#include "Shader.hpp"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vertexFile;
  std::ifstream fragmentFile;

  vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);

    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    vertexCode = vertexStream.str();
    fragmentCode = fragmentStream.str();

    vertexFile.close();
    fragmentFile.close();

  } catch (std::ifstream::failure &e) {
    spdlog::error("Shader file fail to read: {}", e.what());
    return;
  }

  const char *vertexCodeCStr = vertexCode.c_str();
  const char *fragmentCodeCStr = fragmentCode.c_str();

  unsigned int vertex, fragment;

  GLCALL(vertex = glCreateShader(GL_VERTEX_SHADER));
  GLCALL(glShaderSource(vertex, 1, &vertexCodeCStr, NULL));
  GLCALL(glCompileShader(vertex));
  CheckCompileErrors(vertex, "VERTEX");

  GLCALL(fragment = glCreateShader(GL_FRAGMENT_SHADER));
  GLCALL(glShaderSource(fragment, 1, &fragmentCodeCStr, NULL));
  GLCALL(glCompileShader(fragment));
  CheckCompileErrors(fragment, "FRAGMENT");

  GLCALL(ID = glCreateProgram());
  GLCALL(glAttachShader(ID, vertex));
  GLCALL(glAttachShader(ID, fragment));
  GLCALL(glLinkProgram(ID));
  CheckCompileErrors(ID, "PROGRAM");

  GLCALL(glDeleteShader(vertex));
  GLCALL(glDeleteShader(fragment));

  spdlog::info("Shader program {} created successfully", ID);
}

Shader::~Shader() {
  GLCALL(glDeleteProgram(ID));
  spdlog::info("Shader program {} deleted", ID);
}

void Shader::Bind() const { GLCALL(glUseProgram(ID)); }
void Shader::UnBind() const { GLCALL(glUseProgram(0)); }

void Shader::CheckCompileErrors(unsigned int shader, const std::string &type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success) {
      GLCALL(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
      spdlog::error("Shader Compilation Error ({}): {}", type, infoLog);
    }
  } else {
    GLCALL(glGetProgramiv(shader, GL_LINK_STATUS, &success));
    if (!success) {
      GLCALL(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
      spdlog::error("Program Linking Error: {}", infoLog);
    }
  }

  if (success) {
    spdlog::info("{} shader compiled successfully", type);
  }
}
void Shader::SetBool(const std::string &name, bool value) const {
  int location = glGetUniformLocation(ID, name.c_str());
  if (location == -1) {
    spdlog::warn("Uniform '{}' not found in shader program {}", name, ID);
  } else {
    GLCALL(glUniform1i(location, static_cast<int>(value)));
  }
}

void Shader::SetFloat(const std::string &name, float value) const {
  int location = glGetUniformLocation(ID, name.c_str());
  if (location == -1) {
    spdlog::warn("Uniform '{}' not found in shader program {}", name, ID);
  } else {
    GLCALL(glUniform1f(location, value));
  }
}

void Shader::SetVec3(const std::string &name, float v1, float v2,
                     float v3) const {
  int location = glGetUniformLocation(ID, name.c_str());
  if (location == -1) {
    spdlog::warn("Uniform '{}' not found in shader program {}", name, ID);
  } else {
    GLCALL(glUniform3f(location, v1, v2, v3));
  }
}

void Shader::SetVec4(const std::string &name, float v1, float v2, float v3,
                     float v4) const {
  int location = glGetUniformLocation(ID, name.c_str());
  if (location == -1) {
    spdlog::warn("Uniform '{}' not found in shader program {}", name, ID);
  } else {
    GLCALL(glUniform4f(location, v1, v2, v3, v4));
  }
}

void Shader::SetInt(const std::string &name, int value) const {
  int location = glGetUniformLocation(ID, name.c_str());
  if (location == -1) {
    spdlog::warn("Uniform '{}' not found in shader program {}", name, ID);
  } else {
    GLCALL(glUniform1i(location, value));
  }
}
void Shader::SetMat4(const std::string &name, const glm::mat4 &model) const {
  int location = glGetUniformLocation(ID, name.c_str());
  if (location == -1) {
    spdlog::warn("Uniform '{}' not found in shader program {}", name, ID);
  } else {
    GLCALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model)));
  }
}
