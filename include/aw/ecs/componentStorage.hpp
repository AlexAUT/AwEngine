#pragma once

#include "aw/config.hpp"
#include "aw/ecs/entity.hpp"
#include "aw/ecs/sparseSet.hpp"

namespace aw::ecs {
class AW_API_EXPORT ComponentStorageBase
{
public:
  virtual ~ComponentStorageBase() = default;

  virtual void removeEntity(Entity e) = 0;
};
template <typename Component>
class AW_API_EXPORT ComponentStorage : public ComponentStorageBase, public SparseSet<Component>
{
  virtual void removeEntity(Entity e)
  {
    if (this->has(e)) {
      this->erase(e);
    }
  }
};
} // namespace aw::ecs
