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

  spdlog::info("Engine initialized successfully");
  return true;
}

void Engine::Run() {
  spdlog::info("Starting main loop");

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
}

void Engine::ShutDown() {
  spdlog::info("Shutting down engine");

  if (window) {
    glfwDestroyWindow(window);
    glfwTerminate();
  }
}
