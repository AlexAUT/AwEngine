#pragma once

#include <aw/ecs/entity.hpp>
#include <aw/ecs/sparseSet.hpp>

namespace aw::ecs
{
class ComponentStorageBase
{
public:
  virtual void removeEntity(Entity e) = 0;
};
template <typename Component>
class ComponentStorage : public ComponentStorageBase, public SparseSet<Component>
{
  virtual void removeEntity(Entity e)
  {
    if (this->has(e))
    {
      this->erase(e);
    }
  }
};
} // namespace aw::ecs
