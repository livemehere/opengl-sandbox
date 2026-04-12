cmake_minimum_required(VERSION 4.2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(OpenglSandbox)

add_subdirectory(engine)
add_subdirectory(playground)

message("ROOT ===============================")
message("Compiler")
message("- ID: ${CMAKE_CXX_COMPILER_ID}")
message("- Version: ${CMAKE_CXX_COMPILER_VERSION}")
message("- Path: ${CMAKE_CXX_COMPILER}")
message("ROOT ===============================")

