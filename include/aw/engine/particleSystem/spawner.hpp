#pragma once

#include "aw/config.hpp"
#include "aw/util/color/colors.hpp"
#include "aw/util/math/vector.hpp"
#include "aw/util/time/time.hpp"

#include <array>
#include <random>

namespace aw {
struct AW_API_EXPORT ParticleSpawner
{
  aw::Seconds startOffset{0.f};

  std::normal_distribution<float> ttl{2.f, 1.f};
  std::normal_distribution<float> interval{1.f, 0.0f};
  std::normal_distribution<float> amount{10.f, 0.f};

  std::array<std::normal_distribution<float>, 3> position{
      std::normal_distribution{0.f, 0.25f}, std::normal_distribution{0.f, 0.25f}, std::normal_distribution{0.f, 0.f}};
  std::normal_distribution<float> size{1.f, 0.f};
  std::array<std::normal_distribution<float>, 2> velocityDir{std::normal_distribution{0.f, 1.f},
                                                             std::normal_distribution{0.f, 1.f}};

  std::array<aw::Color, 2> colorGradient{aw::Colors::GreenYellow, aw::Colors::BlueViolet};
};
} // namespace aw
