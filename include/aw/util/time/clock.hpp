#pragma once

#include <aw/util/time/gameClock.hpp>
#include <aw/util/time/time.hpp>

namespace aw {
class Clock
{
public:
  GameClock::duration restart();

  GameClock::duration getElapsedTime() const;

private:
  GameClock::time_point mStartPoint{GameClock::now()};
};
} // namespace aw
