#include <aw/util/log.hpp>

#ifdef __ANDROID__
#include "spdlog/sinks/android_sink.h"

#include <memory>
void setupAndroidLogger()
{
  std::string tag = "spdlog-android";
  auto android_logger = spdlog::android_logger_st("android", tag);
  android_logger->critical("Use \"adb shell logcat\" to view this message.");
  spdlog::set_default_logger(android_logger);
}
#endif

namespace aw::log::priv {
bool init()
{
#ifdef __ANDROID__
  setupAndroidLogger();
#endif
  return true;
}
} // namespace aw::log::priv
