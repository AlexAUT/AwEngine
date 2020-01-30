#include "../doctest.h"

#include <aw/ecs/entity.hpp>
#include <aw/ecs/world.hpp>

#include <aw/ecs/view.hpp>

#include <algorithm>
#include <string>

namespace testComponentIterator {
struct Position
{
  int x;
};
struct Name
{
  std::string name;
};
} // namespace testComponentIterator

TEST_CASE("View without matching entities should return same begin as end")
{
  using namespace testComponentIterator;
  aw::ecs::World w;
  w.registerComponent<Position>();

  aw::ecs::View<Position> view(w);
  CHECK(view.begin() == view.end());

  int count = 0;
  for (auto _ : view) {
    count++;
  }
  CHECK(count == 0);
}

TEST_CASE("Test component iteration")
{
  using namespace testComponentIterator;
  aw::ecs::World w;
  w.registerComponent<Position>();

  auto e1 = w.createEntity();
  auto e2 = w.createEntity();

  w.set<Position>(e2, Position{2});
  w.set<Position>(e1, Position{1});

  aw::ecs::View<Position> view(w);

  auto checkContent = [&]() {
    int index = 0;
    for (auto [e, p] : view) {
      if (index == 0) {
        CHECK(p.x == 2);
        CHECK(e == e2);
      } else if (index == 1) {
        CHECK(p.x == 1);
        CHECK(e == e1);
      } else {
        CHECK(false);
      }
      index++;
    }
  };

  checkContent();
  w.unset<Position>(e1);

  checkContent();

  w.set<Position>(e1, Position{1});
  w.unset<Position>(e2);

  int index = 0;
  for (auto [e, p] : view) {
    if (index == 0) {
      CHECK(p.x == 1);
      CHECK(e == e1);
    } else {
      CHECK(false);
    }
    index++;
  }
}

TEST_CASE("Test multi component iteration")
{
  using namespace testComponentIterator;

  aw::ecs::World w;

  w.registerComponent<Position>();
  w.registerComponent<Name>();

  auto e1 = w.createEntity();
  auto e2 = w.createEntity();
  auto e3 = w.createEntity();

  w.set<Position>(e1, Position{1});
  w.set<Position>(e2, Position{2});
  w.set<Position>(e3, Position{3});

  w.set<Name>(e3, Name{std::to_string(3)});
  w.set<Name>(e1, Name{std::to_string(1)});
  w.set<Name>(e2, Name{std::to_string(2)});

  aw::ecs::View<Position, Name> view(w);
  std::vector<bool> visited = {false, false, false};
  for (auto [e, p, n] : view) {
    if (e == e1) {
      CHECK(p.x == 1);
      CHECK(n.name == std::to_string(1));
    }
    if (e == e2) {
      CHECK(p.x == 2);
      CHECK(n.name == std::to_string(2));
    }
    if (e == e3) {
      CHECK(p.x == 3);
      CHECK(n.name == std::to_string(3));
    }
    visited[e.index()] = true;
  }
  CHECK(std::all_of(visited.begin(), visited.end(), [](auto v) { return v; }));

  w.unset<Name>(e2);
  visited = {false, true, false};
  for (auto [e, p, n] : view) {
    if (e == e1) {
      CHECK(p.x == 1);
      CHECK(n.name == std::to_string(1));
    }
    if (e == e2) {
      CHECK(false);
    }
    if (e == e3) {
      CHECK(p.x == 3);
      CHECK(n.name == std::to_string(3));
    }
    visited[e.index()] = true;
  }
  CHECK(std::all_of(visited.begin(), visited.end(), [](auto v) { return v; }));
}
