#pragma once

#include "../Component.hpp"
#include "../Entity.hpp"

class Movement : public Component {
public:
  Movement() = default;
  ~Movement() = default;

  float speed = 10.0f;

  void Update(float deltaTime) override {
    auto transform = owner->GetComponent<Transform>();
    transform->rotation.y += speed * deltaTime;
    transform->rotation.z += speed * deltaTime;
  };
};
