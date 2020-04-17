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

ParticleSystem::ParticleSystem(entt::registry& registry) : mRegistry{&registry}
{
  mRegistry->on_construct<ParticleSpawner>().connect<&ParticleSystem::onSpawnerConstruct>(*this);
}

ParticleSystem::~ParticleSystem()
{
  mRegistry->on_construct<ParticleSpawner>().disconnect<&ParticleSystem::onSpawnerConstruct>(*this);
}

void ParticleSystem::update(aw::Seconds dt, View view)
{
  mSimulationTime += dt;

  view.each([this](entt::entity /*unused*/, ParticleSpawner& spawner, SpawnerState& state) {
    if (state.nextSpawn <= mSimulationTime) {
      auto diff = (mSimulationTime - state.nextSpawn).count();

      int amount = static_cast<int>(std::round(sample(spawner.amount)));
      for (int i = 0; i < amount; i++) {
        // Substract diff to account for delayed spawning
        auto ttl = sample(spawner.ttl) - diff;
        if (ttl <= 0.f) {
          continue;
        }

        auto aliveUntil = mSimulationTime.count() + ttl;
        Particle particle{
            aw::Vec3{sample(spawner.position[0]), sample(spawner.position[1]), sample(spawner.position[2])},
            sample(spawner.size), aw::Vec2{sample(spawner.velocityDir[0]), sample(spawner.velocityDir[1])}, aliveUntil,
            ttl};
        mCreateList.emplace_back(particle);
      }
      state.nextSpawn += aw::Seconds{sample(spawner.interval)};
    }

    auto& particleContainer = mParticles[state.particleIndex];
    auto& particles = particleContainer.particles;

    // Update gradient
    particleContainer.colorGradient = spawner.colorGradient;

    // Remove dead particles
    // Try to replace them by the new particles first
    auto oneBefore = std::next(particles.begin(), particles.size() - 1);
    for (auto it = particles.begin(); it != particles.end(); it++) {
      if (it->aliveUntil <= mSimulationTime.count()) {
        if (!mCreateList.empty()) {
          *it = mCreateList.back();
          mCreateList.pop_back();
        } else {
          if (it != oneBefore) {
            *it = particles.back();
            it--;
            particles.pop_back();
          } else {
            particles.pop_back();
            break;
          }
        }
      }
    }
    // Insert the still pending ones
    particles.insert(particles.end(), mCreateList.begin(), mCreateList.end());
    mCreateList.clear();
  });
}

auto ParticleSystem::particles() const -> const std::vector<ParticleContainer>&
{
  return mParticles;
}

auto ParticleSystem::simulationTime() const -> aw::Seconds
{
  return mSimulationTime;
}

void ParticleSystem::onSpawnerConstruct(entt::registry& registry, entt::entity entity)
{
  auto& spawner = registry.get<ParticleSpawner>(entity);

  // TODO handle multiple with removal
  auto index = mParticles.size();
  mParticles.emplace_back();
  ParticleSystem::SpawnerState state{spawner.startOffset + aw::Seconds{sample(spawner.interval)}, index};

  registry.assign_or_replace<ParticleSystem::SpawnerState>(entity, state);
}

} // namespace aw
