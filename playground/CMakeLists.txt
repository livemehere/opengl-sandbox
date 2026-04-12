
message("playground start ===============================")

file(GLOB_RECURSE PLAYGROUND_SOURCES CONFIGURE_DEPENDS
  src/*.cpp
)

# own source
add_executable(playground ${PLAYGROUND_SOURCES})
target_include_directories(playground PUBLIC include)
target_link_libraries(playground PUBLIC engine)

message("playground end ===============================")

