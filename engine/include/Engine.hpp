#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Engine {
private:
  GLFWwindow *window = nullptr;
  void LogHardwareInfo();

public:
  Engine();
  ~Engine();

  bool Init(int width, int height, const char *title);
  void Run();
  void ShutDown();
};
