#include <aw/util/time/clock.hpp>

namespace aw {
GameClock::duration Clock::restart()
{
  GameClock::time_point oldStart = mStartPoint;
  mStartPoint = GameClock::now();
  return mStartPoint - oldStart;
}

GameClock::duration Clock::getElapsedTime() const
{
  GameClock::time_point now = GameClock::now();
  return now - mStartPoint;
}
} // namespace aw
