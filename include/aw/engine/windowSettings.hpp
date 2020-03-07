#pragma once

#include "aw/config.hpp"

#include <aw/util/math/vector.hpp>

#include <string>

namespace aw {
struct AW_API_EXPORT WindowSettings
{
  std::string title;

  aw::Vec2u resolution;
};
} // namespace aw
