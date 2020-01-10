#pragma once

#include <aw/ecs/componentStorage.hpp>
#include <aw/ecs/entity.hpp>
#include <aw/ecs/systemGroup.hpp>

#include <array>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace aw::ecs
{
class World
{
public:
  enum class SystemType
  {
    OnUpdate,
    OnInsert,
    OnDelete,
    Count
  };
  constexpr static unsigned systemTypeCount = static_cast<unsigned>(SystemType::Count);

public:
  Entity createEntity();
  void destroyEntity(Entity e);
  bool alive(Entity e);

  template <typename Component>
  void set(Component&& component);

  template <typename Component>
  void set(const Component& component);

  std::size_t aliveEntities() const;

private:
private:
  std::array<SystemGroup, systemTypeCount> mSystemGroups;

  std::vector<Entity> mEntities;
  std::size_t mAliveEntities{0};

  constexpr static Entity::Index mFreeListEnd = (1 << (Entity::indexBits - 1));
  Entity::Index mFreeListBegin{mFreeListEnd};

  std::vector<std::unique_ptr<ComponentStorage>> mComponentStorages;

  std::unordered_map<std::string_view, std::size_t> mComponentLookup;
};
} // namespace aw::ecs

namespace aw::ecs
{
template <typename Component>
void World::set(Component&& component)
{
}

template <typename Component>
void World::set(const Component& component)
{
}

} // namespace aw::ecs
