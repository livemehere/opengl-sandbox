#pragma once

#include <glad/glad.h>
#include <string>

inline std::string GetGLErrorString(GLenum errorCode) {
  std::string error = "Unknown Error";
  switch (errorCode) {
  case GL_INVALID_ENUM:
    error = "INVALID_ENUM";
    break;
  case GL_INVALID_VALUE:
    error = "INVALID_VALUE";
    break;
  case GL_INVALID_OPERATION:
    error = "INVALID_OPERATION";
    break;
  case GL_STACK_OVERFLOW:
    error = "STACK_OVERFLOW";
    break;
  case GL_STACK_UNDERFLOW:
    error = "STACK_UNDERFLOW";
    break;
  case GL_OUT_OF_MEMORY:
    error = "OUT_OF_MEMORY";
    break;
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    error = "INVALID_FRAMEBUFFER_OPERATION";
    break;
  }

  return error;
}

#define GLCALL(x)                                                              \
  do {                                                                         \
    while (glGetError() != GL_NO_ERROR)                                        \
      ;                                                                        \
    x;                                                                         \
    if (GLenum error = glGetError()) {                                         \
      std::string errorStr = GetGLErrorString(error);                          \
      spdlog::error("[OpenGL Error] ({}): {} {} at line {}", errorStr, #x,     \
                    __FILE__, __LINE__);                                       \
    }                                                                          \
  } while (0)
