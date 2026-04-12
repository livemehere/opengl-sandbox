#pragma once

#include <GLFW/glfw3.h>

class Engine {
private:
  GLFWwindow *window = nullptr;

public:
  Engine();
  ~Engine();

  bool Init(int width, int height, const char *title);
  void Run();
  void ShutDown();
};
