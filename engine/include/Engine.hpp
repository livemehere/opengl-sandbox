#pragma once

#define GLFW_INCLUDE_NONE
#include "Debug.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>

class Engine {
private:
  GLFWwindow *window = nullptr;
  int width;
  int height;
  int fbWidth;
  int fbHeight;
  void LogHardwareInfo();
  static void ErrorCallback(int error, const char *desc) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, desc);
  }

public:
  Engine();
  ~Engine();

  bool Init(int width, int height, const char *title);
  void Run();
  void ShutDown();
  void SetSize(int width, int height);
};
