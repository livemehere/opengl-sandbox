#include "Shader.hpp"
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

  spdlog::info("Vertex Shader Code:\n{}", vertexCode);
  spdlog::info("Fragment Shader Code:\n{}", fragmentCode);
}

Shader::~Shader() = default;
