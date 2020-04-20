#pragma once

#include "aw/config.hpp"
#include "aw/engine/particleSystem/system.hpp"
#include "aw/util/color/color.hpp"
#include "aw/util/math/matrix.hpp"
#include "aw/util/time/time.hpp"

namespace aw {
class PathRegistry;
}

namespace aw {
class AW_API_EXPORT ParticleRenderer
{
public:
  using Gradient = std::array<aw::Color, 2>;

public:
  ParticleRenderer(const aw::PathRegistry& pathRegistry, aw::Vec2i windowSize);
  ~ParticleRenderer();

  ParticleRenderer(const ParticleRenderer&) = delete;
  auto operator=(const ParticleRenderer&) -> ParticleRenderer& = delete;

  void render(const aw::Mat4& vp, aw::Seconds simulationTime,
              const std::vector<aw::ParticleSystem::ParticleContainer>& particles);

  void windowResize(aw::Vec2i windowSize);

private:
  void colorGradient(Gradient gradient, float fadeIn);

private:
  Gradient mColorGradient;

  unsigned mVertexVbo{};
  unsigned mParticleVbo{};
  unsigned mColorGradientTexture{};

  unsigned mVao{};

  unsigned mProgram{};
  int mVpLoc{-1};
  int mSimTimeLoc{-1};
  int mGradientTextureLoc{-1};
  float mFadeIn{-1.f};
};
} // namespace aw
