#include "../doctest.h"

#include <aw/ecs/world.hpp>

TEST_CASE("Entities can be created")
{
  aw::ecs::World world;

  CHECK(world.aliveEntities() == 0);

  auto e1 = world.createEntity();
  CHECK(world.aliveEntities() == 1);

  auto e2 = world.createEntity();
  CHECK(world.aliveEntities() == 2);

  auto e3 = world.createEntity();
  CHECK(world.aliveEntities() == 3);

  CHECK(e1 != e2);
  CHECK(e2 != e3);
  CHECK(e1 != e3);
}

TEST_CASE("Entities can be destroyed")
{
  aw::ecs::World world;

  CHECK(world.aliveEntities() == 0);

  auto e1 = world.createEntity();
  CHECK(world.aliveEntities() == 1);

  auto e2 = world.createEntity();
  CHECK(world.aliveEntities() == 2);

  auto e3 = world.createEntity();
  CHECK(world.aliveEntities() == 3);

  SUBCASE("In the same order as created")
  {
    world.destroyEntity(e1);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 2);
    CHECK(!world.alive(e1));
    world.destroyEntity(e2);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 1);
    CHECK(!world.alive(e1));
    CHECK(!world.alive(e2));
    world.destroyEntity(e3);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 0);
    CHECK(!world.alive(e1));
    CHECK(!world.alive(e2));
    CHECK(!world.alive(e3));
  }
  SUBCASE("In the reverse order as created")
  {
    world.destroyEntity(e3);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 2);
    world.destroyEntity(e2);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 1);
    world.destroyEntity(e1);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 0);
  }
  SUBCASE("In random order")
  {
    world.destroyEntity(e2);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 2);
    world.destroyEntity(e1);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 1);
    world.destroyEntity(e3);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 0);

    SUBCASE("Double destroy with empty world should not do anything")
    {
      e2 = world.createEntity();
      world.destroyEntity(e2);
      world.update(aw::Seconds{0.f});
      CHECK(world.aliveEntities() == 0);
      world.destroyEntity(e2);
      world.update(aw::Seconds{0.f});
      CHECK(world.aliveEntities() == 0);
    }
  }
  SUBCASE("Double destroy should not do anything")
  {
    world.destroyEntity(e2);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 2);
    world.destroyEntity(e2);
    world.update(aw::Seconds{0.f});
    CHECK(world.aliveEntities() == 2);
  }
}

TEST_CASE("Entities can be reused after destroy")
{
  aw::ecs::World world;

  auto e1 = world.createEntity();

  world.destroyEntity(e1);
  world.update(aw::Seconds{0.f});

  auto e1New = world.createEntity();
  CHECK(world.aliveEntities() == 1);
  CHECK(e1 != e1New);
  CHECK(e1.index() == e1New.index());
}

TEST_CASE("Entities should be reused after destroy in reverse order")
{
  aw::ecs::World world;
  auto e1 = world.createEntity();
  auto e2 = world.createEntity();
  auto e3 = world.createEntity();
  auto e4 = world.createEntity();

  world.destroyEntity(e2);
  world.destroyEntity(e3);
  world.destroyEntity(e1);

  world.update(aw::Seconds{0.f});

  auto e1New = world.createEntity();
  CHECK(e1New.index() == e1.index());
  CHECK(e1New != e1);

  auto e2New = world.createEntity();
  CHECK(e2New.index() == e3.index());
  CHECK(e2New != e3);

  auto e3New = world.createEntity();
  CHECK(e3New.index() == e2.index());
  CHECK(e3New != e2);
}

