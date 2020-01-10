#pragma once

#include <atomic>

namespace aw::type
{
template <typename Category, typename CounterType = unsigned>
class Counter
{
public:
  template <typename T>
  inline static CounterType id()
  {
    return generateId<typename std::decay_t<T>>();
  }

private:
  template <typename T>
  inline static CounterType generateId()
  {
    static CounterType typeId = mCounter.fetch_add(1);
    return typeId;
  }

private:
  inline static std::atomic<CounterType> mCounter{0};
};
} // namespace aw::type
