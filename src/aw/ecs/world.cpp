#include <aw/ecs/world.hpp>

#include <aw/ecs/view.hpp>

#include <cassert>

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

bool World::alive(Entity e) const
{
  return e.valid() && mEntities.size() > e.index() && mEntities[e.index()] == e;
}

std::size_t World::aliveEntities() const
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
}
} // namespace aw::ecs
