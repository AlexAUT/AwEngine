#pragma once

#include "aw/config.hpp"
#include <random>

namespace aw {
template <typename DataType>
class ClampedNormalDist
{
public:
  explicit ClampedNormalDist(DataType constant) : mMin{constant}, mMax{constant}, mDist{constant, 0.f} {}

  ClampedNormalDist(DataType min, DataType max) :
      mMin{min}, mMax{max}, mDist{(min + max) / static_cast<DataType>(2), (max - min) / static_cast<DataType>(6)}
  {
    assert(min <= max);
  }

  // Repeats the sampling if out of min/max. With default mean/stddev the loops happens 0.03% of the time
  template <typename Generator>
  auto operator()(Generator& g) -> DataType
  {
    DataType result;
    do {
      result = mDist(g);
    } while (result > mMax || result < mMin);

    return result;
  }

  auto min() const -> DataType { return mMin; }
  auto max() const -> DataType { return mMax; }

private:
  DataType mMin;
  DataType mMax;
  std::normal_distribution<DataType> mDist;
};
}
