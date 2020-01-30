#pragma once

#include <spdlog/spdlog.h>

namespace aw::log {
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

// TODO find better place
#include <aw/util/math/vector.hpp>
template <>
struct fmt::formatter<aw::Vec3>
{
  // Parses format specifications of the form ['f' | 'e'].
  static constexpr auto parse(format_parse_context& ctx)
  {
    auto it = ctx.begin();
    return it;
  }
  // Formats the point p using the parsed format specification (presentation)
  // stored in this formatter.
  template <typename FormatContext>
  auto format(const aw::Vec3& v, FormatContext& ctx)
  {
    // ctx.out() is an output iterator to write to.
    return format_to(ctx.out(), "Vec3({}, {}, {})", v.x, v.y, v.z);
  }
};
