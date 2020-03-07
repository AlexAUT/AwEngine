#pragma once

#include "aw/util/time/gameClock.hpp"
#include "aw/util/time/time.hpp"

namespace aw {
class Clock
{
public:
  using duration = GameClock::duration;

public:
  auto restart() -> GameClock::duration;
  auto getElapsedTime() const -> GameClock::duration;

private:
  GameClock::time_point mStartPoint{GameClock::now()};
};
} // namespace aw
