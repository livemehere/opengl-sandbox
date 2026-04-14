#include "Shader.hpp"
#include "Debug.hpp"
#include <fstream>
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
