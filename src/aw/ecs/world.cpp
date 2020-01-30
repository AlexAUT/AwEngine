#include <aw/ecs/world.hpp>

#include <cassert>
#include <utility>

namespace aw::ecs {
Entity World::createEntity()
{
  assert(mEntities.size() >= mAliveEntities);

  mAliveEntities++;
  if (mFreeListBegin == mFreeListEnd) {
    mEntities.emplace_back(mEntities.size(), 0);

    return mEntities.back();
  } else {
    auto index = mFreeListBegin;
    // Update free list begin and restore valid entity id
    mFreeListBegin = mEntities[index].index();
    assert(mFreeListBegin <= mFreeListEnd);
    mEntities[index] = Entity(index, mEntities[index].counter() + 1);
    assert(mEntities[index].index() <= mFreeListEnd);
    return mEntities[index];
  }
}

void World::destroyEntity(Entity e)
{
  assert(e.valid());
  mEraseList.push_back(e);
}

auto World::alive(Entity e) const -> bool
{
  return e.valid() && mEntities.size() > e.index() && mEntities[e.index()] == e;
}

auto World::aliveEntities() const -> std::size_t
{
  return mAliveEntities;
}

void World::update(aw::Seconds dt)
{
  for (auto e : mEraseList) {
    if (!alive(e)) {
      continue;
    }

    for (auto& storage : mComponentStorages) {
      storage->removeEntity(e);
    }

    mEntities[e.index()] = Entity(mFreeListBegin, mEntities[e.index()].counter());
    assert(mEntities[e.index()].index() == mFreeListBegin);
    mFreeListBegin = e.index();
    assert(mFreeListBegin <= mFreeListEnd);
    mAliveEntities--;
  }
  mEraseList.clear();

  // Update systems
  systems(SystemType::OnUpdate).update(dt);
}

void World::render()
{
  systems(SystemType::OnRender).update(aw::Seconds{0.f});
}

auto World::systems(SystemType type) -> SystemGroup&
{
  assert(type != SystemType::Count);
  return mSystemGroups[static_cast<std::size_t>(type)];
}

void World::registerSystem(SystemType type, std::function<void(aw::Seconds)> func)
{
  systems(type).add(std::move(func));
}

} // namespace aw::ecs
