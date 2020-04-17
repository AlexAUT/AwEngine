#pragma once

#include "aw/engine/particleSystem/spawner.hpp"
#include "aw/util/serialization/types/random.hpp"
#include "cereal/macros.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/chrono.hpp"

namespace cereal {
template <typename Archive>
void serialize(Archive& ar, aw::ParticleSpawner& s, std::size_t version)
{
    ar(cereal::make_nvp("startOffset", s.startOffset), cereal::make_nvp("ttl", s.ttl),
       cereal::make_nvp("interval", s.interval), cereal::make_nvp("amount", s.amount),
       cereal::make_nvp("position", s.position), cereal::make_nvp("size", s.size),
       cereal::make_nvp("velocityDir", s.velocityDir), cereal::make_nvp("colorGrad", s.colorGradient));
}
} // namespace cereal

