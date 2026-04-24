#include "Engine.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

Engine::Engine() = default;
Engine::~Engine() = default;

bool Engine::Init(int width, int height, const char *title) {
  if (!glfwInit()) {
    spdlog::error("Failed to initialize GLFW");
    return false;
  }

  glfwSetErrorCallback(ErrorCallback);

  // Apple gives us OpenGL 4.1, even though we ask for 3.3.
  // That's ok - 4.1 is backward compatible with 3.3.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window) {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::error("Failed to initialize GLAD");
    return false;
  }

  LogHardwareInfo();

  spdlog::info("Engine initialized successfully");
  return true;
}

void Engine::Run() {
  spdlog::info("Starting main loop");

  Shader shader("../../../shaders/basic.vs", "../../../shaders/basic.fs");
  // clang-format off
  Mesh mesh(
      {
          {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // top-left
          {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},  // top-right
          {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // bottom-right
          {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}  // bottom-left
      },
      {
          0, 1, 2, // first triangle
          2, 3, 0  // second triangle
      });
  // clang-format on
  Texture texture("../../../assets/texture.jpg");

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // move
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      position.x -= 0.01f;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      position.x += 0.01f;
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      position.y += 0.01f;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      position.y -= 0.01f;
    }

    // scale
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
      scale.x += 0.01f;
      scale.y += 0.01f;
    } else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
      scale.x -= 0.01f;
      scale.y -= 0.01f;
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);

    shader.Bind();
    texture.Bind();
    shader.SetBool("uUseTexture", true);
    shader.SetInt("uTexture", 0);
    shader.SetMat4("uModel", model);

    mesh.Bind();
    mesh.Draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  shader.UnBind();
  mesh.UnBind();
}

void Engine::ShutDown() {
  spdlog::info("Shutting down engine");

  if (window) {
    glfwDestroyWindow(window);
    glfwTerminate();
  }
}

void Engine::LogHardwareInfo() {
  auto vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
  auto renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
  auto version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  auto glslVersion =
      reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

  spdlog::info("-----------------------------------------");
  spdlog::info("GPU Vendor: {}", vendor);
  spdlog::info("GPU Renderer: {}", renderer);
  spdlog::info("OpenGL Version: {}", version);
  spdlog::info("GLSL Version: {}", glslVersion);
  spdlog::info("-----------------------------------------");
}
