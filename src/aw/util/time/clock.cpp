#include "aw/util/time/clock.hpp"

namespace aw {
auto Clock::restart() -> GameClock::duration
{
  GameClock::time_point oldStart = mStartPoint;
  mStartPoint = GameClock::now();
  return mStartPoint - oldStart;
}

auto Clock::getElapsedTime() const -> GameClock::duration
{
  GameClock::time_point now = GameClock::now();
  return now - mStartPoint;
}
} // namespace aw
