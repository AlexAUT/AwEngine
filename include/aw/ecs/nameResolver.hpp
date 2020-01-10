#pragma once

#include <string_view>
#include <typeinfo>

namespace aw::ecs
{
template <typename T>
std::string_view getComponentName();

template <typename T>
std::string_view getComponentName()
{
  return typeof(T).Name;
}
} // namespace aw::ecs
