
message("engine start ===============================")

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
  src/*.cpp
)

# own source
add_library(engine STATIC ${ENGINE_SOURCES})
target_include_directories(engine PUBLIC include)

# external


message("engine end ===============================")

