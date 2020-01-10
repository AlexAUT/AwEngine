#include <aw/ecs/world.hpp>

#include <cassert>

namespace aw::ecs
{
Entity World::createEntity()
{
  assert(mEntities.size() >= mAliveEntities);

  mAliveEntities++;
  if (mFreeListBegin == mFreeListEnd)
  {
    mEntities.emplace_back(mEntities.size(), 0);

    return mEntities.back();
  }
  else
  {
    auto index = mFreeListBegin;
    // Update free list begin and restore valid entity id
    mFreeListBegin = mEntities[index].index();
    assert(mFreeListBegin <= mFreeListEnd);
    mEntities[index] = Entity(index, mEntities[index].counter() + 1);
    assert(mEntities[index].index() <= mFreeListEnd);
    return mEntities[index];
  }
}

void World::destroyEntity(Entity e) {}

bool World::alive(Entity e)
{
  return false;
}

std::size_t World::aliveEntities() const
{
  return mAliveEntities;
}
} // namespace aw::ecs
