#pragma once

#include "aw/util/math/matrix.hpp"
#include "aw/util/math/vector.hpp"
#include "glm/geometric.hpp"
namespace aw {

struct Sphere
{
  aw::Vec3 center{0.f};
  float radius{0.f};

  auto transformBy(const aw::Mat4& transform) -> Sphere
  {
    auto newRadius = glm::length(transform * aw::Vec4{radius, 0.f, 0.f, 0.f});
    return Sphere{aw::Vec3{transform * aw::Vec4{center, 1.f}}, newRadius};
  }
};
} // namespace aw
