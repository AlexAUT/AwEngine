#pragma once

#include <spdlog/spdlog.h>

namespace aw::log
{
template <typename... Args>
void debug(const char* format, Args&&... args)
{
  spdlog::debug(format, std::forward<Args>(args)...);
}

template <typename... Args>
void error(const char* format, Args&&... args)
{
  spdlog::error(format, std::forward<Args>(args)...);
}

template <typename... Args>
void info(const char* format, Args&&... args)
{
  spdlog::info(format, std::forward<Args>(args)...);
}

template <typename... Args>
void warn(const char* format, Args&&... args)
{
  spdlog::warn(format, std::forward<Args>(args)...);
}
} // namespace aw::log
