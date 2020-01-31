#pragma once

#include "aw/util/time/clock.hpp"
#include "aw/util/time/gameClock.hpp"
#include "aw/util/time/time.hpp"

#include <cstdint>
#include <string>

namespace aw::perf {
class Section
{
public:
  Section(std::string title);
  ~Section();

  void start();
  void end();

  void print();
  void printLast();

private:
private:
  std::string mTitle;
  aw::Clock mClock;

  aw::Clock::duration mSampleSum{aw::Clock::duration::zero()};
  aw::Clock::duration mLast{0};
  std::size_t mSampleCount{0};
};
} // namespace aw::perf
