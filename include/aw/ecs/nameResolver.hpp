#pragma once

#include "aw/config.hpp"

#include <string_view>
#include <typeinfo>

namespace aw::ecs {
template <typename T>
AW_API_EXPORT auto getComponentName() -> std::string_view;

template <typename T>
AW_API_EXPORT auto getComponentName() -> std::string_view
{
  return typeid(T).name();
}
} // namespace aw::ecs
