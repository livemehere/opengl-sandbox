#include "Engine.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Transform.hpp"
#include <vector>

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
  spdlog::info("Starting main loop");

  ImGuiIO &io = ImGui::GetIO();

  Camera camera;
  camera.transform.position.z = 8.0f;
  camera.transform.position.y = 2.0f;
  camera.target = glm::vec3(0.0f, 0.0f, 0.0f);

  Shader shader("../../../shaders/basic.vs", "../../../shaders/basic.fs");

  std::vector<Entity> entities;

  Mesh mesh = Mesh::CreateCube();
  Texture texture("../../../assets/texture.jpg");

  for (int i = 0; i < 5; i++) {
    Entity box(&mesh, &texture);
    box.transform.position = glm::vec3(i * 1.5f - 3.0f, 0.0f, 0.0f);
    entities.push_back(box);
  }
  auto &box = entities[0];

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // z-order
  glEnable(GL_DEPTH_TEST);

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
    ImGui::SliderFloat("Position X", &box.transform.position.x, 0.0f,
                       static_cast<float>(width));
    ImGui::SliderFloat("Position Y", &box.transform.position.y, 0.0f,
                       static_cast<float>(height));
    ImGui::SliderFloat("Position Z", &box.transform.position.z, -100.0f,
                       100.0f);

    ImGui::Separator();

    ImGui::Text("Rotate");
    ImGui::SliderFloat("Rotation X", &box.transform.rotation.x, -180.0f,
                       180.0f);
    ImGui::SliderFloat("Rotation Y", &box.transform.rotation.y, -180.0f,
                       180.0f);
    ImGui::SliderFloat("Rotation Z", &box.transform.rotation.z, -180.0f,
                       180.0f);

    ImGui::End();

    camera.Update();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

    for (auto &entity : entities) {
      entity.Draw(view, proj, shader);
      entity.transform.rotation.x += 0.25f;
      entity.transform.rotation.y += 0.25f;
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    camera.Controls(window);

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
