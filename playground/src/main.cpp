#include "Engine.hpp"

int main() {

  Engine engine;

  if (!engine.Init(800, 600, "OpenGL Playgorund")) {
    return -1;
  };

  engine.Run();

  engine.ShutDown();

  return 0;
}
