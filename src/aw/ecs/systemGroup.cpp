#include <aw/ecs/view.hpp>

#include <aw/ecs/systemGroup.hpp>

namespace aw::ecs {

auto SystemGroup::add(SystemFunc system) -> SystemId
{
  auto id = ++mIdCounter;
  mIds.push_back(id);
  mSystems.push_back(std::move(system));

  return id;
}
void SystemGroup::update(aw::Seconds dt)
{
  for (auto& system : mSystems) {
    system(dt);
  }
}
} // namespace aw::ecs
