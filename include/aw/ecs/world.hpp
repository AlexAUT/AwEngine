#pragma once

#include <aw/ecs/componentStorage.hpp>
#include <aw/ecs/entity.hpp>
#include <aw/ecs/nameResolver.hpp>
#include <aw/ecs/sparseSet.hpp>
#include <aw/ecs/systemGroup.hpp>
#include <aw/ecs/view.hpp>
#include <aw/util/time/time.hpp>

#include <array>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace aw::ecs {
class World
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
  Entity createEntity();
  void destroyEntity(Entity e);
  bool alive(Entity e) const;
  std::size_t aliveEntities() const;

  template <typename Component>
  void registerComponent();

  template <typename Component>
  bool registered() const;

  template <typename Component>
  bool has(Entity e);

  template <typename Component>
  Component& get(Entity e);

  template <typename Component, typename... Args>
  Component& set(Entity e, Args&&... args);

  template <typename Component>
  void unset(Entity e);

  SystemGroup& systems(SystemType);

  /* template <typename... Components> */
  /* void registerSystem(SystemType, std::function<void(aw::Seconds, Entity, Components&...)>); */

  /* template <typename... Components> */
  /* void registerSystem(SystemType type, std::function<void(aw::Seconds, View<Components...>)> func); */

  template <typename System>
  void registerSystem(SystemType type, System& system);

  void update(aw::Seconds dt);
  void render();

  template <typename Component>
  ComponentStorage<Component>& getStorage();
  template <typename Component>
  ComponentStorage<Component>& getStorage() const;

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
bool World::has(Entity e)
{
  assert(registered<Component>());
  assert(alive(e));
  return getStorage<Component>().has(e);
}

template <typename Component>
Component& World::get(Entity e)
{
  assert(registered<Component>());
  assert(alive(e));
  assert(has<Component>(e));
  return *getStorage<Component>().get(e);
}

template <typename Component, typename... Args>
Component& World::set(Entity e, Args&&... args)
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
bool World::registered() const
{
  return mComponentLookup.find(getComponentName<Component>()) != mComponentLookup.end();
}

template <typename Component>
ComponentStorage<Component>& World::getStorage()
{
  assert(registered<Component>());
  auto index = mComponentLookup.find(getComponentName<Component>())->second;
  return *static_cast<ComponentStorage<Component>*>(mComponentStorages[index].get());
}

template <typename Component>
ComponentStorage<Component>& World::getStorage() const
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
  systems(type).add(std::function([& world = *this, &system = system](aw::Seconds dt) {
    using View = typename System::View;
    View view{world};
    system.update(dt, view);
  }));
}

} // namespace aw::ecs
