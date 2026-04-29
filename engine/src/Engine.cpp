#include "Engine.hpp"
#include "Camera/Camera.hpp"
#include "ecs/Entity.hpp"
#include "ecs/components/Mesh.hpp"
#include "ecs/components/Movement.hpp"
#include "resources/Geometry.hpp"
#include "resources/Material.hpp"
#include "resources/Shader.hpp"
#include "resources/Texture.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <memory>
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

  std::vector<std::unique_ptr<Entity>> entities;

  Camera camera;
  camera.transform.position.z = 8.0f;
  camera.transform.position.y = 2.0f;

  Geometry cubeGeometry = Geometry::CreateCube();

  Shader shader("../../../shaders/basic.vs", "../../../shaders/basic.fs");
  Texture texture("../../../assets/texture.jpg");
  Material material(&shader, &texture);

  for (int i = 0; i < 5; i++) {
    auto entity = std::make_unique<Entity>();
    auto transform = entity->GetComponent<Transform>();
    transform->position = glm::vec3(i * 1.5f - 3.0f, 0.0f, 0.0f);
    entity->AddComponent<Mesh>(&cubeGeometry, &material);
    entity->AddComponent<Movement>();
    entities.push_back(std::move(entity));
  }
  auto selectedTransform = entities[0]->GetComponent<Transform>();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // z-order
  glEnable(GL_DEPTH_TEST);

  float lastFrame = 0.0f;
  float deltaTime = 0.0f;

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
    ImGui::SliderFloat("Position X", &selectedTransform->position.x, 0.0f,
                       static_cast<float>(width));
    ImGui::SliderFloat("Position Y", &selectedTransform->position.y, 0.0f,
                       static_cast<float>(height));
    ImGui::SliderFloat("Position Z", &selectedTransform->position.z, -100.0f,
                       100.0f);

    ImGui::Separator();

    ImGui::Text("Rotate");
    ImGui::SliderFloat("Rotation X", &selectedTransform->rotation.x, -180.0f,
                       180.0f);
    ImGui::SliderFloat("Rotation Y", &selectedTransform->rotation.y, -180.0f,
                       180.0f);
    ImGui::SliderFloat("Rotation Z", &selectedTransform->rotation.z, -180.0f,
                       180.0f);

    ImGui::End();

    camera.Update();
    camera.Controls(window);

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

    Mesh::SetRenderMatrices(view, proj);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    for (auto &entity : entities) {
      entity->Update(deltaTime);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  shader.UnBind();
  cubeGeometry.UnBind();
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
