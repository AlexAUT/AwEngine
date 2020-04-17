#pragma once

#include "cereal/cereal.hpp"

#include <random>

namespace cereal {
template <typename Archive, typename DataType>
void save(Archive& ar, const std::normal_distribution<DataType>& dist)
{
  ar(cereal::make_nvp("mean", dist.mean()), cereal::make_nvp("stddev", dist.stddev()));
}

template <typename Archive, typename DataType>
void load(Archive& ar, std::normal_distribution<DataType>& dist)
{
  DataType mean;
  DataType stddev;
  ar(CEREAL_NVP(mean), CEREAL_NVP(stddev));
  dist = std::normal_distribution(mean, stddev);
}
} // namespace cereal
