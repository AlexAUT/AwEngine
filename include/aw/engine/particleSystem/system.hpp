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
    float ttl;
    aw::Vec3 position;
    float size;
  };

  struct AW_API_EXPORT SpawnerState
  {
    aw::Seconds nextSpawn;
  };

public:
  using View = entt::basic_view<entt::entity, entt::exclude_t<>, ParticleSpawner, SpawnerState>;

  ParticleSystem(entt::registry& registry);

  void update(aw::Seconds dt, View view);

  auto particles() const -> const std::vector<Particle>&;

private:
  entt::registry& mRegistry;

  std::vector<Particle> mParticles;

  std::vector<Particle> mCreateList;

  aw::Seconds mSimulationTime{0.f};
};
} // namespace aw
