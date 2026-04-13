
message("playground start ===============================")

file(GLOB_RECURSE PLAYGROUND_SOURCES CONFIGURE_DEPENDS
  src/*.cpp
)

# own source
add_executable(playground ${PLAYGROUND_SOURCES})
target_include_directories(playground PUBLIC include)
target_link_libraries(playground PUBLIC engine)

add_custom_command(TARGET playground POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
  ${CMAKE_SOURCE_DIR}/shaders
  $<TARGET_FILE_DIR:playground>/shaders
  COMMENT "Copying shaders to build directory"
)

message("playground end ===============================")

