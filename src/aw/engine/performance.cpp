#include "aw/util/log.hpp"
#include "aw/util/time/time.hpp"

#include <aw/engine/performance.hpp>

namespace aw::perf {
Section::Section(std::string title) : mTitle{std::move(title)} {}

Section::~Section()
{
  print();
}

void Section::start()
{
  mClock.restart();
}

void Section::end()
{
  mLast = mClock.getElapsedTime();
  mSampleSum += mLast;
  mSampleCount++;
}

void Section::print()
{
  if (mSampleCount > 0) {
    APP_INFO("Perf section {}: avg: {}s", mTitle,
             (std::chrono::duration_cast<aw::Seconds>(mSampleSum).count() / mSampleCount));
  }
}

void Section::printLast()
{
  APP_INFO("Perf section {}: last: {}s", mTitle, std::chrono::duration_cast<aw::Seconds>(mLast).count());
}
} // namespace aw::perf
