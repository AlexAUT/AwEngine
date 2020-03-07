#pragma once

#include "aw/config.hpp"
#include "aw/util/filesystem/filesystem.hpp"
#include "spdlog/logger.h"

#include <memory>

namespace aw {
auto appLog() -> spdlog::logger&;
auto engineLog() -> spdlog::logger&;

namespace priv::log {
auto init(std::string appName, const fs::path& logFolder) -> bool;
} // namespace priv::log
} // namespace aw

#define APP_ERROR(...) aw::appLog().error(__VA_ARGS__);
#define APP_WARN(...) aw::appLog().warn(__VA_ARGS__);
#define APP_INFO(...) aw::appLog().info(__VA_ARGS__);
#define APP_DEBUG(...) aw::appLog().debug(__VA_ARGS__);
#define APP_TRACE(...) aw::appLog().trace(__VA_ARGS__);

#define ENGINE_ERROR(...) aw::engineLog().error(__VA_ARGS__);
#define ENGINE_WARN(...) aw::engineLog().warn(__VA_ARGS__);
#define ENGINE_INFO(...) aw::engineLog().info(__VA_ARGS__);
#define ENGINE_DEBUG(...) aw::engineLog().debug(__VA_ARGS__);
#define ENGINE_TRACE(...) aw::engineLog().trace(__VA_ARGS__);

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

template <>
struct fmt::formatter<aw::Vec4>
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
  auto format(const aw::Vec4& v, FormatContext& ctx)
  {
    // ctx.out() is an output iterator to write to.
    return format_to(ctx.out(), "Vec4({}, {}, {} {})", v.x, v.y, v.z, v.w);
  }
};
