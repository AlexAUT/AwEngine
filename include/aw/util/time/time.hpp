#pragma once

#include <aw/util/time/gameClock.hpp>

#include <chrono>

namespace aw {
using Seconds = std::chrono::duration<float>;
using MilliSeconds = std::chrono::duration<float, std::milli>;
using MicroSeconds = std::chrono::duration<float, std::micro>;
using NanoSeconds = std::chrono::duration<float, std::nano>;

using TimePoint = GameClock::time_point;
} // namespace aw
