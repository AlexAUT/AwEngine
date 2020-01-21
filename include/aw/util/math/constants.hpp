#pragma once

namespace aw {
template <typename Type = float>
constexpr Type pi()
{
  return 3.14159265358979;
}
template <typename Type = float>
constexpr Type pi_2()
{
  return pi<Type>() / 2.f;
}
template <typename Type = float>
constexpr Type pi_4()
{
  return pi<Type>() / 4.f;
}

template <typename Type = float>
constexpr Type to_rad()
{
  return pi<Type>() / 180.f;
}
template <typename Type = float>
constexpr Type to_deg()
{
  return 180.f / pi<Type>();
}
} // namespace aw
