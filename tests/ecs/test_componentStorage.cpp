#include "../doctest.h"

#include <aw/ecs/world.hpp>

#include <string>

struct TestComponentStorage
{
  std::string name;
};

TEST_CASE("Registering component type")
{
  aw::ecs::World world;

  CHECK(!world.registered<TestComponentStorage>());

  world.registerComponent<TestComponentStorage>();

  CHECK(world.registered<TestComponentStorage>());
}

TEST_CASE("Registered components can be set")
{
  aw::ecs::World world;

  world.registerComponent<TestComponentStorage>();
  auto e1 = world.createEntity();

  std::string text{"Hallo"};

  CHECK(!world.has<TestComponentStorage>(e1));
  auto c = world.set<TestComponentStorage>(e1, text);

  CHECK(world.has<TestComponentStorage>(e1));
  CHECK(c.name == text);
}

TEST_CASE("Registered components can be removed")
{
  aw::ecs::World world;

  world.registerComponent<TestComponentStorage>();
  auto e1 = world.createEntity();

  world.set<TestComponentStorage>(e1, "Hallo");
  world.unset<TestComponentStorage>(e1);

  CHECK(!world.has<TestComponentStorage>(e1));
}

TEST_CASE("Removing should not affect other components")
{
  aw::ecs::World world;

  world.registerComponent<TestComponentStorage>();
  auto e1 = world.createEntity();
  auto e2 = world.createEntity();

  world.set<TestComponentStorage>(e1, "Hallo1");
  world.set<TestComponentStorage>(e2, "Hallo2");

  world.unset<TestComponentStorage>(e1);

  CHECK(!world.has<TestComponentStorage>(e1));
  CHECK(world.has<TestComponentStorage>(e2));
  CHECK(world.get<TestComponentStorage>(e2).name == "Hallo2");
}

TEST_CASE("Removing entity should remove components")
{
  // Test by reinserting after delete
  aw::ecs::World world;

  world.registerComponent<TestComponentStorage>();
  auto e1 = world.createEntity();

  world.set<TestComponentStorage>(e1, "Hallo1");

  world.destroyEntity(e1);
  world.update(aw::Seconds{0.f});

  auto e1New = world.createEntity();
  CHECK(e1.index() == e1New.index());
  CHECK(e1 != e1New);
  CHECK(!world.alive(e1));
  CHECK(!world.has<TestComponentStorage>(e1New));
}
