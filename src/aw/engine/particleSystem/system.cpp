#include "aw/engine/particleSystem/system.hpp"

#include "aw/engine/particleSystem/spawner.hpp"
#include "aw/util/log.hpp"
#include "entt/entity/registry.hpp"
#include "entt/entity/view.hpp"

#include <random>

namespace aw {
std::mt19937 mGenerator;

template <typename Distribution>
auto sample(Distribution distribution)
{
  return distribution(mGenerator);
}

void onSpawnerConstruct(entt::registry& reg, entt::entity entity)
{
  auto& spawner = reg.get<ParticleSpawner>(entity);
  ParticleSystem::SpawnerState state{spawner.startOffset + aw::Seconds{sample(spawner.interval)}};

  reg.assign<ParticleSystem::SpawnerState>(entity, state);
}

ParticleSystem::ParticleSystem(entt::registry& registry) : mRegistry{registry}
{
  mRegistry.on_construct<ParticleSpawner>().connect<&aw::onSpawnerConstruct>();
}

void ParticleSystem::update(aw::Seconds dt, View view)
{
  mSimulationTime += dt;

  view.each([this](entt::entity /*unused*/, ParticleSpawner& spawner, SpawnerState& state) {
    if (state.nextSpawn <= mSimulationTime) {
      state.nextSpawn += aw::Seconds{sample(spawner.interval)};

      int amount = static_cast<int>(std::round(sample(spawner.amount)));
      for (int i = 0; i < amount; i++) {
        Particle particle{
            sample(spawner.ttl),
            aw::Vec3{sample(spawner.position[0]), sample(spawner.position[1]), sample(spawner.position[2])},
            sample(spawner.size)};
        mCreateList.emplace_back(particle);
      }
    }
  });

  // Remove dead particles
  // Try to replace them by the new particles first
  auto oneBefore = std::next(mParticles.begin(), mParticles.size() - 1);
  for (auto it = mParticles.begin(); it != mParticles.end(); it++) {
    it->ttl -= dt.count();
    if (it->ttl <= 0.f) {
      if (!mCreateList.empty()) {
        *it = mCreateList.back();
        mCreateList.pop_back();
      } else {
        if (it != oneBefore) {
          *it = mParticles.back();
          it--;
          mParticles.pop_back();
        } else {
          mParticles.pop_back();
          break;
        }
      }
    }
  }
  // Insert the still pending ones
  mParticles.insert(mParticles.end(), mCreateList.begin(), mCreateList.end());
  mCreateList.clear();
}

auto ParticleSystem::particles() const -> const std::vector<Particle>&
{
  return mParticles;
}

} // namespace aw
