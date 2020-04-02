#pragma once

#include "aw/config.hpp"
#include "aw/util/math/vector.hpp"
#include "aw/util/time/time.hpp"

#include <array>
#include <random>

namespace aw {
struct AW_API_EXPORT ParticleSpawner
{
  aw::Seconds startOffset{0.f};

  std::normal_distribution<float> ttl{3.f, 0.f};
  std::normal_distribution<float> interval;
  std::normal_distribution<float> amount{1.f, 0.f};

  std::array<std::normal_distribution<float>, 2> position;
  std::normal_distribution<float> size{1.f, 0.f};
};
} // namespace aw
