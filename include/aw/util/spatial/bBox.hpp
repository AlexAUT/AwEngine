#pragma once

#include <aw/util/math/matrix.hpp>
#include <aw/util/math/vector.hpp>

#include <glm/common.hpp>

#include <limits>
#include <ostream>

namespace aw {
class BBox
{
public:
  BBox() = default;
  explicit BBox(float size);
  explicit BBox(Vec3 size);
  BBox(Vec3 origin, Vec3 size);

  auto size() const -> Vec3 { return mMax - mMin; }
  auto extend() const -> Vec3 { return size() * 0.5f; }
  auto center() const -> Vec3 { return mMin + extend(); }

  auto min() -> Vec3 { return mMin; }
  auto max() -> Vec3 { return mMax; }

  void operator+=(const Vec3& point)
  {
    mMin = glm::min(mMin, point);
    mMax = glm::max(mMax, point);
  }

  void addPoint(const Vec3& point) { (*this) += point; }

private:
  Vec3 mMin{std::numeric_limits<float>::max()};
  Vec3 mMax{std::numeric_limits<float>::lowest()};
};
} // namespace aw
