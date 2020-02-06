#pragma once

#include <aw/ecs/entity.hpp>
#include <aw/ecs/world.hpp>

#include <tuple>
#include <type_traits>

namespace aw::ecs {

class Prefab
{
public:
  Prefab() = default;
  virtual ~Prefab() = default;

  Prefab(const Prefab&) = default;
  Prefab(Prefab&&) = default;
  auto operator=(const Prefab&) -> Prefab& = default;
  auto operator=(Prefab &&) -> Prefab& = default;

  virtual auto spawn(aw::ecs::World& world) -> aw::ecs::Entity = 0;
};

template <typename... Components>
class TuplePrefab : public Prefab
{
public:
  TuplePrefab(std::tuple<Components...> components) : components(components) {}

  auto spawnRetComponents(aw::ecs::World& world)
  {
    auto e = world.createEntity();

    auto setToWorld = [e, &world](auto& c) {
      using Component = std::decay_t<decltype(c)>;
      return world.set<Component>(e, c);
    };
    auto setAll = [&](auto&... components) { return std::make_tuple(setToWorld(components)...); };

    auto cInstances = std::apply(setAll, components);
    return std::tuple_cat(std::make_tuple(e), cInstances);
  }

  auto spawn(aw::ecs::World& world) -> aw::ecs::Entity final
  {
    return std::get<aw::ecs::Entity>(spawnRetComponents(world));
  }

private:
  std::tuple<Components...> components;
};

} // namespace aw::ecs
