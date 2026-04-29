#pragma once

class Entity;

class Component {
protected:
  Entity *owner = nullptr;

  friend class Entity;

public:
  virtual ~Component() = default;
  virtual void Update(float deltaTime) {};
};
