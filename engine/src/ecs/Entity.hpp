#pragma once

#include "Component.hpp"
#include "components/Transform.hpp"
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

class Entity {

private:
  std::vector<Component *> componentList;
  std::unordered_map<std::type_index, std::unique_ptr<Component>> componentMap;

public:
  Entity() { AddComponent<Transform>(); }

  ~Entity() = default;

  template <typename T, typename... TArgs> T *AddComponent(TArgs &&...args) {
    auto component = std::make_unique<T>(std::forward<TArgs>(args)...);
    component->owner = this;
    T *ptr = component.get();
    componentList.push_back(ptr);
    componentMap[std::type_index(typeid(T))] = std::move(component);

    return ptr;
  }

  template <typename T> T *GetComponent() {
    auto it = componentMap.find(std::type_index(typeid(T)));
    if (it != componentMap.end()) {
      return static_cast<T *>(it->second.get());
    }
    return nullptr;
  }

  void Update(float deltaTime) {
    for (auto *component : componentList) {
      component->Update(deltaTime);
    }
  }
};
