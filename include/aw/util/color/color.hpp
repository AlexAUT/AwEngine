#pragma once

namespace aw {
struct Color
{
  float r, g, b, a;

  template <typename Archive>
  void serialize(Archive& ar)
  {
    ar(r, g, b, a);
  }
};

inline auto operator==(const Color& a, const Color& b) -> bool
{
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

inline auto operator!=(const Color& a, const Color& b) -> bool
{
  return !(a == b);
}
} // namespace aw
