
message("engine start ===============================")

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
  src/*.cpp
  src/*.c
)

# own source
add_library(engine STATIC ${ENGINE_SOURCES})
target_include_directories(engine PUBLIC include)

# external
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(engine PUBLIC glfw spdlog::spdlog OpenGL::GL imgui::imgui)

message("engine end ===============================")

