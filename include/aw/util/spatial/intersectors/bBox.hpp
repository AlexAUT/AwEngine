#pragma once

#include "aw/util/log.hpp"
#include "aw/util/spatial/bBox.hpp"
#include "aw/util/spatial/plane.hpp"
#include "aw/util/spatial/sphere.hpp"
#include <glm/gtx/extended_min_max.hpp>

namespace aw {

template <typename VectorType>
struct BBoxPointIntersector
{
  auto operator()(BBox box, VectorType point) -> bool
  {
    return (point.x >= box.min().x && point.x <= box.max().x) && (point.y >= box.min().y && point.y <= box.max().y) &&
           (point.z >= box.min().z && point.z <= box.max().z);
  }
};

struct BBoxBBoxIntersector
{
  auto operator()(BBox a, BBox b) -> bool
  {
    return (a.min().x <= b.max().x && a.max().x >= b.min().x) && (a.min().y <= b.max().y && a.max().y >= b.min().y) &&
           (a.min().z <= b.max().z && a.max().z >= b.min().z);
  }
};

struct BBoxPlaneIntersector
{
  enum class Result
  {
    Intersecting,
    Positive, // The whole bbox lies in front of the plane
    Negative  // The whole bbox lies behind the plane
  };

  Result operator()(BBox box, Plane plane)
  {
    auto center = box.center();
    auto ext = box.extend();

    auto r = glm::dot(ext, glm::abs(plane.n));
    float s = glm::dot(plane.n, center) - plane.w;

    if (s > r) {
      return Result::Positive;
    }
    if (s < r) {
      return Result::Negative;
    }
    return Result::Intersecting;
  }
};

struct BBoxSphereIntersector
{
  auto operator()(BBox box, Sphere sphere) -> bool
  {
    float dmin = 0.f;
    for (int i = 0; i < 3; i++) {
      if (sphere.center[i] < box.min()[i]) {
        dmin += glm::pow(sphere.center[i] - box.min()[i], 2);
      } else if (sphere.center[i] > box.max()[i]) {
        dmin += glm::pow(sphere.center[i] - box.max()[i], 2);
      }
    }

    return dmin <= sphere.radius * sphere.radius;
  }
};

struct BBoxTriangleIntersector
{
  auto operator()(BBox bounds, Vec3 t1, Vec3 t2, Vec3 t3) -> bool
  {
    // Move the bbox into the origin
    const auto bExt = bounds.extend();
    auto bCenter = bounds.center();
    t1 -= bCenter;
    t2 -= bCenter;
    t3 -= bCenter;

    bCenter -= bCenter;

    // Edges
    const auto e1 = t2 - t1;
    const auto e2 = t3 - t2;
    const auto e3 = t1 - t3;

    // Face normals of BBox
    const Vec3 u1{1.f, 0.f, 0.f};
    const Vec3 u2{0.f, 1.f, 0.f};
    const Vec3 u3{0.f, 0.f, 1.f};

    using namespace glm;
    // Test edges of BBox
    const auto axis11 = cross(u1, t1);
    const auto axis12 = cross(u1, t2);
    const auto axis13 = cross(u1, t3);

    const auto axis21 = cross(u2, t1);
    const auto axis22 = cross(u2, t2);
    const auto axis23 = cross(u2, t3);

    const auto axis31 = cross(u3, t1);
    const auto axis32 = cross(u3, t2);
    const auto axis33 = cross(u3, t3);

    // Test axiss11
    // Project points
    float p0 = dot(t1, axis11);
    float p1 = dot(t2, axis11);
    float p2 = dot(t3, axis11);
    float r = bExt.y * abs(e1.z) + bExt.z * abs(e1.y);
    if (max(-max(p0, p1, p2), min(p0, p1, p2)) > r) {
      return false;
    }

    p0 = dot(t1, axis12);
    p1 = dot(t2, axis12);
    p2 = dot(t3, axis12);
    r = bExt.y * abs(e2.z) + bExt.z * abs(e2.y);
    if (max(-max(p0, p1, p2), min(p0, p1, p2)) > r) {
      return false;
    }

    p0 = dot(t1, axis13);
    p1 = dot(t2, axis13);
    p2 = dot(t3, axis13);
    r = bExt.y * abs(e3.z) + bExt.z * abs(e3.y);
    if (max(-max(p0, p1, p2), min(p0, p1, p2)) > r) {
      return false;
    }

    p0 = dot(t1, axis21);
    p1 = dot(t2, axis21);
    p2 = dot(t3, axis21);
    r = bExt.x * abs(e1.z) + bExt.z * abs(e1.x);
    if (max(-max(p0, p1, p2), min(p0, p1, p2)) > r) {
      return false;
    }

    p0 = dot(t1, axis22);
    p1 = dot(t2, axis22);
    p2 = dot(t3, axis22);
    r = bExt.x * abs(e2.z) + bExt.z * abs(e2.x);
    if (max(-max(p0, p1, p2), min(p0, p1, p2)) > r) {
      return false;
    }

    p0 = dot(t1, axis23);
    p1 = dot(t2, axis23);
    p2 = dot(t3, axis23);
    r = bExt.x * abs(e3.z) + bExt.z * abs(e3.x);
    if (max(-max(p0, p1, p2), min(p0, p1, p2)) > r) {
      return false;
    }

    p0 = dot(t1, axis31);
    p1 = dot(t2, axis31);
    p2 = dot(t3, axis31);
    r = bExt.x * abs(e1.y) + bExt.y * abs(e1.x);
    if (max(-max(p0, p1, p2), min(p0, p1, p2)) > r) {
      return false;
    }

    p0 = dot(t1, axis32);
    p1 = dot(t2, axis32);
    p2 = dot(t3, axis32);
    r = bExt.x * abs(e2.y) + bExt.y * abs(e2.x);
    if (max(-max(p0, p1, p2), min(p0, p1, p2)) > r) {
      return false;
    }

    p0 = dot(t1, axis33);
    p1 = dot(t2, axis33);
    p2 = dot(t3, axis33);
    r = bExt.x * abs(e3.y) + bExt.y * abs(e3.x);
    if (max(-max(p0, p1, p2), min(p0, p1, p2)) > r) {
      return false;
    }

    if (max(t1.x, t2.x, t3.x) < -bExt.x || min(t1.x, t2.x, t3.x) > bExt.x) {
      return false;
    }

    if (max(t1.y, t2.y, t3.y) < -bExt.y || min(t1.y, t2.y, t3.y) > bExt.y) {
      return false;
    }

    if (max(t1.z, t2.z, t3.z) < -bExt.z || min(t1.z, t2.z, t3.z) > bExt.z) {
      return false;
    }

    // Check face plane
    const auto planeNormal = cross(e1, e2);
    const auto planeDistance = dot(planeNormal, t1);

    r = bExt.x * abs(planeNormal.x) + bExt.y * abs(planeNormal.y) + bExt.z * abs(planeNormal.z);

    return planeDistance <= r;
  }
};
} // namespace aw
