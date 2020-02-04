#pragma once

#include "aw/util/math/matrix.hpp"
#include "aw/util/math/vector.hpp"
#include "glm/gtx/matrix_operation.hpp"

namespace aw {
struct Plane
{
  aw::Vec3 n;
  float w;

  auto transformBy(const aw::Mat4& transform) -> Plane
  {
    auto adjugate = (glm::adjugate(transform));
    auto det = glm::determinant(transform);
    return transformBy(transform, adjugate, det);
  }
  auto transformBy(const aw::Mat4& transform, const aw::Mat4& adjugate, float det) -> Plane
  {
    auto newNorm = glm::normalize(aw::Vec3{adjugate * aw::Vec4{n, 0.f}});
    if (det < 0.f) {
      newNorm = -newNorm;
    }
    auto invW = transform * aw::Vec4{n * w, 1.f};
    auto newW = glm::dot(aw::Vec3{invW}, newNorm);

    return Plane{newNorm, newW};
  }

  auto distance(aw::Vec3 point) -> float { return glm::dot(n, point) - w; }
};
} // namespace aw
