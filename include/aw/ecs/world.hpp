#pragma once

#include "aw/config.hpp"
#include "aw/ecs/componentStorage.hpp"
#include "aw/ecs/entity.hpp"
#include "aw/ecs/nameResolver.hpp"
#include "aw/ecs/sparseSet.hpp"
#include "aw/ecs/systemGroup.hpp"
#include "aw/ecs/view.hpp"
#include "aw/util/time/time.hpp"
#include "aw/util/type/staticFor.hpp"

#include <array>
#include <memory>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace aw::ecs {
class AW_API_EXPORT World
{
public:
  enum class SystemType
  {
    OnUpdate,
    OnRender,
    OnInsert,
    OnDelete,
    Count
  };
  constexpr static unsigned systemTypeCount = static_cast<unsigned>(SystemType::Count);

public:
  auto createEntity() -> Entity;
  void destroyEntity(Entity e);
  auto alive(Entity e) const -> bool;
  auto aliveEntities() const -> std::size_t;

  template <typename Component>
  void registerComponent();

  template <typename Component>
  auto registered() const -> bool;

  template <typename Component>
  auto has(Entity e) -> bool;

  template <typename Component>
  auto get(Entity e) -> Component&;

  template <typename Component>
  auto set(Entity e, Component&& component);

  template <typename Component, typename... Args>
  auto set(Entity e, Args&&... args) -> Component&;

  template <typename Component>
  void unset(Entity e);

  auto systems(SystemType) -> SystemGroup&;

  void registerSystem(SystemType type, std::function<void(aw::Seconds)> func);

  template <typename System>
  void registerSystem(SystemType type, System& system);

  void update(aw::Seconds dt);
  void render();

  template <typename Component>
  auto getStorage() -> ComponentStorage<Component>&;
  template <typename Component>
  auto getStorage() const -> ComponentStorage<Component>&;

private:
private:
  std::array<SystemGroup, systemTypeCount> mSystemGroups;

  std::vector<Entity> mEntities;
  std::size_t mAliveEntities{0};

  constexpr static Entity::Index mFreeListEnd = (1 << (Entity::indexBits - 1));
  Entity::Index mFreeListBegin{mFreeListEnd};

  std::vector<std::unique_ptr<ComponentStorageBase>> mComponentStorages;

  std::unordered_map<std::string_view, std::size_t> mComponentLookup;

  std::vector<Entity> mEraseList;
};
} // namespace aw::ecs

namespace aw::ecs {
template <typename Component>
auto World::has(Entity e) -> bool
{
  assert(registered<Component>());
  assert(alive(e));
  return getStorage<Component>().has(e);
}

template <typename Component>
auto World::get(Entity e) -> Component&
{
  assert(registered<Component>());
  assert(alive(e));
  assert(has<Component>(e));
  return *getStorage<Component>().get(e);
}

template <typename Component>
auto World::set(Entity e, Component&& component)
{
  assert(registered<Component>());
  assert(alive(e));
  return getStorage<std::decay_t<Component>>().emplace(e, component);
}

template <typename Component, typename... Args>
auto World::set(Entity e, Args&&... args) -> Component&
{
  assert(registered<Component>());
  assert(alive(e));
  return getStorage<Component>().emplace(e, std::forward<Args>(args)...);
}

template <typename Component>
void World::unset(Entity e)
{
  assert(registered<Component>());
  assert(alive(e));
  getStorage<Component>().erase(e);
}

template <typename Component>
void World::registerComponent()
{
  assert(!registered<Component>());
  assert(mComponentStorages.size() == mComponentLookup.size());

  mComponentLookup.emplace(getComponentName<Component>(), mComponentStorages.size());
  mComponentStorages.emplace_back(std::make_unique<ComponentStorage<Component>>());
}

template <typename Component>
auto World::registered() const -> bool
{
  return mComponentLookup.find(getComponentName<Component>()) != mComponentLookup.end();
}

template <typename Component>
auto World::getStorage() -> ComponentStorage<Component>&
{
  assert(registered<Component>());
  auto index = mComponentLookup.find(getComponentName<Component>())->second;
  return *static_cast<ComponentStorage<Component>*>(mComponentStorages[index].get());
}

template <typename Component>
auto World::getStorage() const -> ComponentStorage<Component>&
{
  assert(registered<Component>());
  auto index = mComponentLookup.find(getComponentName<Component>())->second;
  return *static_cast<ComponentStorage<Component>*>(mComponentStorages[index].get());
}

/* template <typename... Components> */
/* void World::registerSystem(SystemType type, std::function<void(aw::Seconds, Entity, Components&...)> callback) */
/* { */
/*   systems(type).add([this, callback](aw::Seconds dt) { */
/*     aw::ecs::View<Components...> view(*this); */
/*     for (auto it : view) { */
/*       std::apply(callback, std::tuple_cat(std::make_tuple(dt), it)); */
/*     } */
/*   }); */
/* } */

/* template <typename... Components> */
/* void World::registerSystem(SystemType type, std::function<void(aw::Seconds, View<Components...>)> func) */
/* { */
/*   systems(type).add([this, func](aw::Seconds dt) { */
/*     aw::ecs::View<Components...> view(*this); */
/*     func(dt, view); */
/*   }); */
/* } */

template <typename System>
void World::registerSystem(SystemType type, System& system)
{
  using ViewList = typename System::Views;

  System::registerComponents(*this);

  constexpr auto viewCount = std::tuple_size_v<ViewList>;
  aw::staticFor<viewCount>([& world = *this, &system = system, type](auto index) {
    world.systems(type).add(std::function([& world = world, &system, index](aw::Seconds dt) {
      using View = std::tuple_element_t<index, ViewList>;
      View view{world};
      system.update(dt, view);
    }));
  });
}

} // namespace aw::ecs
