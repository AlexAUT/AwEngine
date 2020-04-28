#include "aw/engine/particleSystem/system.hpp"

#include "aw/engine/particleSystem/spawner.hpp"
#include "aw/util/log.hpp"
#include "aw/util/math/transform.hpp"
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

  view.each([this](entt::entity /*unused*/, ParticleSpawner& spawner, SpawnerState& state, Transform& transform) {
    while (state.nextSpawn <= mSimulationTime) {

      auto diff = (mSimulationTime - state.nextSpawn).count();

      auto instantSpawner = (spawner.interval.min() == 0.f && spawner.interval.max() == 0.f);
      if (instantSpawner) {
        diff = 0.f;
      }

      int amount = static_cast<int>(std::round(sample(spawner.amount)));
      for (int i = 0; i < amount; i++) {
        // Substract diff to account for delayed spawning
        auto ttl = sample(spawner.ttl) - diff;
        if (ttl <= 0.f) {
          continue;
        }

        auto aliveUntil = mSimulationTime.count() + ttl;
        auto pos = transform.position() +
                   aw::Vec3{sample(spawner.position[0]), sample(spawner.position[1]), sample(spawner.position[2])};

        //        auto pos1 = transform.transform() * aw::Vec4{sample(spawner.position[0]), sample(spawner.position[1]),
        //                                                     sample(spawner.position[2]), 1.0};

        auto velocity =
            transform.transform() * aw::Vec4{sample(spawner.velocityDir[0]), 0.f, sample(spawner.velocityDir[1]), 0.f};
        velocity.y = velocity.z;

        Particle particle{aw::Vec3{pos}, sample(spawner.size),    aw::Vec2{velocity}, aliveUntil,
                          ttl,           sample(spawner.rotation)};
        mCreateList.emplace_back(particle);
      }
      state.nextSpawn += std::max(aw::Seconds{sample(spawner.interval)}, aw::Seconds{0.f});
      if (instantSpawner) {
        break;
      }
    }

    auto& particleContainer = mParticles[state.particleIndex];
    auto& particles = particleContainer.particles;

    // Update gradient
    particleContainer.colorGradient = spawner.colorGradient;
    particleContainer.fadeInTime = spawner.fadeIn;

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
  assert(registry.has<aw::Transform>(entity) && "A particle spawner will not work without a transform component!");

  auto& spawner = registry.get<ParticleSpawner>(entity);

  // TODO handle multiple with removal
  auto index = mParticles.size();
  mParticles.emplace_back();
  ParticleSystem::SpawnerState state{spawner.startOffset + aw::Seconds{sample(spawner.interval)}, index};

  registry.assign_or_replace<ParticleSystem::SpawnerState>(entity, state);
}

} // namespace aw
