#include "aw/util/math/transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace aw {
Transform::Transform(aw::Vec3 pos, aw::Vec3 scale, aw::Quat rot) : mPosition{pos}, mScale{scale}, mRotation{rot}
{
  updateTransform();
}

void Transform::position(aw::Vec3 pos)
{
  mPosition = pos;
  updateTransform();
}

auto Transform::position() const -> aw::Vec3
{
  return mPosition;
}

void Transform::scale(aw::Vec3 scale)
{
  mScale = scale;
  updateTransform();
}

auto Transform::scale() const -> aw::Vec3
{
  return mScale;
}

void Transform::rotation(aw::Quat rotation)
{
  mRotation = rotation;
  updateTransform();
}

auto Transform::rotation() const -> aw::Quat
{
  return mRotation;
}

auto Transform::transform() const -> const aw::Mat4&
{
  return mTransform;
}

void Transform::updateTransform()
{
  mTransform = aw::Mat4(1.f);
  aw::Mat4 scaling = glm::scale(mTransform, mScale);
  aw::Mat4 rotation = glm::mat4_cast(mRotation);
  aw::Mat4 translation = glm::translate(mTransform, mPosition);
  mTransform = translation * rotation * scaling;
}
}
