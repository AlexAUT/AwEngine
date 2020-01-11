#include "../doctest.h"

#include <aw/ecs/world.hpp>

TEST_CASE("System can be added")
{
  aw::ecs::World world;
  world.registerComponent<unsigned>();

  auto amount = 100u;

  // Create entities
  for (auto i = 0u; i < amount; i++) {
    auto e = world.createEntity();
    world.set<unsigned>(e, i);
  }

  unsigned index = 0;
  using Type = aw::ecs::World::SystemType;
  world.registerSystem(Type::OnUpdate, std::function([&](aw::Seconds dt, aw::ecs::Entity e, unsigned& i) {
                         CHECK(index == i);
                         index++;
                       }));

  world.update(aw::Seconds{0.f});

  CHECK(index == amount);
}
