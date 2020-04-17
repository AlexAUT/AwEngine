#pragma once

#include "aw/config.hpp"
#include "aw/util/math/vector.hpp"
#include "aw/util/time/time.hpp"
#include "entt/entity/fwd.hpp"
#include "spawner.hpp"

namespace aw {
class AW_API_EXPORT ParticleSystem
{
public:
  struct AW_API_EXPORT Particle
  {
    aw::Vec3 position;
    float size;
    aw::Vec2 velocity;
    float aliveUntil;
    float aliveFor;
  };

  struct AW_API_EXPORT SpawnerState
  {
    aw::Seconds nextSpawn;
    std::size_t particleIndex;
  };

  struct ParticleContainer
  {
    std::array<aw::Color, 2> colorGradient;
    std::vector<Particle> particles;
  };

public:
  using View = entt::basic_view<entt::entity, entt::exclude_t<>, ParticleSpawner, SpawnerState>;

  ParticleSystem(entt::registry& registry);
  ~ParticleSystem();

  auto operator=(ParticleSystem&&) noexcept -> ParticleSystem&;

  void update(aw::Seconds dt, View view);

  auto particles() const -> const std::vector<ParticleContainer>&;
  auto simulationTime() const -> aw::Seconds;

private:
  void onSpawnerConstruct(entt::registry& registry, entt::entity entity);

private:
  entt::registry* mRegistry;

  std::vector<ParticleContainer> mParticles;

  std::vector<Particle> mCreateList;

  aw::Seconds mSimulationTime{0.f};
};
} // namespace aw
