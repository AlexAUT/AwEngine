#pragma once

#include <chrono>

namespace aw
{
class GameClock
{
public:
  using InternalClock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                           std::chrono::high_resolution_clock, std::chrono::steady_clock>;

  using rep = typename InternalClock::rep;
  using period = typename InternalClock::period;
  using duration = typename InternalClock::duration;
  using time_point = typename InternalClock::time_point;

  constexpr static bool is_steady = InternalClock::is_steady;

  static inline auto start{InternalClock::now()};

  static auto now()
  {
    auto now = InternalClock::now();
    return start + (now - start);
  }
};
} // namespace aw
