#include "Engine.hpp"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

Engine::Engine() = default;
Engine::~Engine() = default;

bool Engine::Init(int width, int height, const char *title) {
  if (!glfwInit()) {
    spdlog::error("Failed to initialize GLFW");
    return false;
  }

  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window) {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  // TODO: glad for windows

  return true;
}

void Engine::Run() {
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
}

void Engine::ShutDown() {
  if (window) {
    glfwDestroyWindow(window);
    glfwTerminate();
  }
}
