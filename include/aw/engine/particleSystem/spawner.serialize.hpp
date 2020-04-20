#pragma once

#include "aw/engine/particleSystem/spawner.hpp"
#include "aw/util/serialization/types/random.hpp"
#include "cereal/macros.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/chrono.hpp"

namespace cereal {
template <typename Archive, typename DataType>
void save(Archive& ar, const aw::ClampedNormalDist<DataType>& dist)
{
  ar(cereal::make_nvp("min", dist.min()), cereal::make_nvp("max", dist.max()));
}

template <typename Archive, typename DataType>
void load(Archive& ar, aw::ClampedNormalDist<DataType>& dist)
{
  DataType min;
  DataType max;
  ar(cereal::make_nvp("min", min), cereal::make_nvp("max", max));
  dist = aw::ClampedNormalDist<DataType>(min, max);
}

template <typename Archive>
void serialize(Archive& ar, aw::ParticleSpawner& s, std::size_t version)
{
  if (version == 3) {
    ar(cereal::make_nvp("startOffset", s.startOffset), cereal::make_nvp("ttl", s.ttl),
       cereal::make_nvp("interval", s.interval), cereal::make_nvp("amount", s.amount),
       cereal::make_nvp("position", s.position), cereal::make_nvp("size", s.size),
       cereal::make_nvp("rotation", s.rotation), cereal::make_nvp("velocityDir", s.velocityDir),
       cereal::make_nvp("colorGrad", s.colorGradient), cereal::make_nvp("fadeIn", s.fadeIn));
  } else if (version == 2) {
    ar(cereal::make_nvp("startOffset", s.startOffset), cereal::make_nvp("ttl", s.ttl),
       cereal::make_nvp("interval", s.interval), cereal::make_nvp("amount", s.amount),
       cereal::make_nvp("position", s.position), cereal::make_nvp("size", s.size),
       cereal::make_nvp("rotation", s.rotation), cereal::make_nvp("velocityDir", s.velocityDir),
       cereal::make_nvp("colorGrad", s.colorGradient));
  } else {
    ar(cereal::make_nvp("startOffset", s.startOffset), cereal::make_nvp("ttl", s.ttl),
       cereal::make_nvp("interval", s.interval), cereal::make_nvp("amount", s.amount),
       cereal::make_nvp("position", s.position), cereal::make_nvp("size", s.size),
       cereal::make_nvp("velocityDir", s.velocityDir), cereal::make_nvp("colorGrad", s.colorGradient));
  }
}
} // namespace cereal
CEREAL_CLASS_VERSION(aw::ParticleSpawner, 3);
