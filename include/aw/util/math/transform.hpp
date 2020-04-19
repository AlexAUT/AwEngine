#pragma once

#include "aw/config.hpp"
#include "aw/util/math/matrix.hpp"
#include "aw/util/math/quat.hpp"
#include "aw/util/math/vector.hpp"

namespace aw {
class AW_API_EXPORT Transform
{
public:
  Transform() = default;
  Transform(aw::Vec3 pos, aw::Vec3 scale = aw::Vec3{1.f}, aw::Quat rot = aw::Quat{});

  void position(aw::Vec3 pos);
  auto position() const -> aw::Vec3;

  void scale(aw::Vec3 scale);
  auto scale() const -> aw::Vec3;

  void rotation(aw::Quat rotation);
  auto rotation() const -> aw::Quat;

  auto transform() const -> const aw::Mat4&;

private:
  void updateTransform();

private:
  aw::Vec3 mPosition{0.f};
  aw::Vec3 mScale{1.f};
  aw::Quat mRotation{};

  aw::Mat4 mTransform{1.f};
};
} // namespace aw
