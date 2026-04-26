#include "Engine.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Engine::Engine() = default;
Engine::~Engine() = default;

void Engine::SetSize(int width, int height) {
  this->width = width;
  this->height = height;
  if (window) {
    glfwSetWindowSize(window, width, height);
  }
}

bool Engine::Init(int width, int height, const char *title) {
  SetSize(width, height);

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

  glfwSetWindowUserPointer(window, this);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::error("Failed to initialize GLAD");
    return false;
  }

  // Set Viewport and Framebuffer Size Callback
  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

  glViewport(0, 0, fbWidth, fbHeight);

  LogHardwareInfo();
  spdlog::info("Engine initialized successfully");

  // IMGUI Setup
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  return true;
}

void Engine::Run() {

  ImGuiIO &io = ImGui::GetIO();

  spdlog::info("Starting main loop");

  Shader shader("../../../shaders/basic.vs", "../../../shaders/basic.fs");
  // clang-format off
  Mesh mesh(
      {
          {{-128.0f, 128.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // top-left
          {{128.0f, 128.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},  // top-right
          {{128.0f, -128.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // bottom-right
          {{-128.0f,-128.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}  // bottom-left
      },
      {
          0, 1, 2, // first triangle
          2, 3, 0  // second triangle
      });
  // clang-format on
  Texture texture("../../../assets/texture.jpg");

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // z-order
  glEnable(GL_DEPTH_TEST);

  // camera
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 400.0f);
  float cameraZoom = 1.0f;

  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

  while (!glfwWindowShouldClose(window)) {
    // logical size
    glfwGetWindowSize(window, &width, &height);
    // physical size
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Controls");
    ImGui::Text("FPS : %.0f", io.Framerate);
    // move
    ImGui::Text("Move");
    ImGui::SliderFloat("Position X", &position.x, 0.0f,
                       static_cast<float>(width));
    ImGui::SliderFloat("Position Y", &position.y, 0.0f,
                       static_cast<float>(height));
    ImGui::SliderFloat("Position Z", &position.z, -100.0f, 100.0f);

    ImGui::Separator();

    ImGui::Text("Rotate");
    ImGui::SliderFloat("Rotation X", &rotation.x, -180.0f, 180.0f);
    ImGui::SliderFloat("Rotation Y", &rotation.y, -180.0f, 180.0f);
    ImGui::SliderFloat("Rotation Z", &rotation.z, -180.0f, 180.0f);

    ImGui::End();

    // move
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      cameraPos.x -= 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      cameraPos.x += 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      cameraPos.y -= 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      cameraPos.y += 1.0f;
    }

    // scale
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
      cameraZoom += 0.1f;
    } else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
      cameraZoom -= 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      SetSize(width += 10, height += 10);
    }

    // model
    glm::mat4 model = glm::mat4(1.0f);

    // translation
    model = glm::translate(model, position);

    // rotation
    model = glm::rotate(model, glm::radians(rotation.x),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z),
                        glm::vec3(0.0f, 0.0f, 1.0f));

    // scale
    model = glm::scale(model, scale);

    // view
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::scale(view, glm::vec3(cameraZoom, cameraZoom, 1.0f));
    view = glm::translate(view, -cameraPos);

    // projection
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

    shader.Bind();
    texture.Bind();
    shader.SetBool("uUseTexture", true);
    shader.SetInt("uTexture", 0);
    shader.SetMat4("uModel", model);
    shader.SetMat4("uView", view);
    shader.SetMat4("uProj", proj);

    mesh.Bind();
    mesh.Draw();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
