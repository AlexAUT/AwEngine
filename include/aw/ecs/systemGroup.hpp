#pragma once

#include <aw/ecs/entity.hpp>
#include <aw/util/time/time.hpp>

#include <functional>

namespace aw::ecs {
class World;

class SystemGroup
{
public:
  using SystemFunc = std::function<void(aw::Seconds)>;
  using SystemId = std::size_t;

public:
  SystemId add(SystemFunc system);

  void update(aw::Seconds dt);

private:
  std::vector<SystemFunc> mSystems;
  std::vector<SystemId> mIds;

  SystemId mIdCounter{0};
};
} // namespace aw::ecs

namespace aw::ecs {} // namespace aw::ecs
