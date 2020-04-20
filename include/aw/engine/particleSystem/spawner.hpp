#pragma once

#include "aw/config.hpp"
#include "aw/util/color/colors.hpp"
#include "aw/util/math/vector.hpp"
#include "aw/util/random/distributions.hpp"
#include "aw/util/time/time.hpp"

#include <algorithm>
#include <array>
#include <random>

namespace aw {
struct AW_API_EXPORT ParticleSpawner
{
  aw::Seconds startOffset{0.f};

  ClampedNormalDist<float> ttl{1.f, 2.f};
  ClampedNormalDist<float> interval{1.f};
  ClampedNormalDist<float> amount{10.f};

  std::array<ClampedNormalDist<float>, 3> position{ClampedNormalDist{0.25f}, ClampedNormalDist{0.25f},
                                                   ClampedNormalDist{0.f}}; // namespace aw

  ClampedNormalDist<float> size{1.f};
  ClampedNormalDist<float> rotation{0.f};
  std::array<ClampedNormalDist<float>, 2> velocityDir{ClampedNormalDist{-1.f, 1.0f}, ClampedNormalDist{-1.f, 1.f}};

  std::array<aw::Color, 2> colorGradient{aw::Colors::GreenYellow, aw::Colors::BlueViolet};
  float fadeIn{0.f};
};
} // namespace aw
