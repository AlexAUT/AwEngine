#include "aw/util/log.hpp"

#include "aw/config.hpp"
#include "fmt/core.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include <memory>

namespace aw {
namespace priv::log {
std::shared_ptr<spdlog::logger> appLogger;
std::shared_ptr<spdlog::logger> engineLogger;

auto init(std::string appName, const fs::path& logPath) -> bool
{
  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  console_sink->set_color_mode(spdlog::color_mode::always);
  console_sink->set_level(spdlog::level::debug);

  auto logFile = logPath / "log.txt";
  auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile.string(), true);
  file_sink->set_level(spdlog::level::trace);

  // or you can even set multi_sink logger as default logger
  engineLogger = std::make_shared<spdlog::logger>("awEngine", spdlog::sinks_init_list({console_sink, file_sink}));
  spdlog::register_logger(engineLogger);

  appLogger = std::make_shared<spdlog::logger>((appName), spdlog::sinks_init_list({console_sink, file_sink}));
  spdlog::set_default_logger(appLogger);

  return true;
}
} // namespace priv::log

AW_API_EXPORT
auto appLog() -> spdlog::logger&
{
  return *priv::log::appLogger;
}

AW_API_EXPORT
auto engineLog() -> spdlog::logger&
{
  return *priv::log::engineLogger;
}
} // namespace aw
