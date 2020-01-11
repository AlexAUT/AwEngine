#pragma once

#include <stdint.h>

namespace aw::ecs {
class Entity
{
public:
  using Identifier = uint32_t;
  using Index = uint32_t;
  using Counter = uint32_t;

  static constexpr Identifier invalidId = static_cast<Identifier>(-1);
  static constexpr Index invalidIndex = (1 << 24) - 1;
  static constexpr unsigned indexBits = 24;
  static constexpr unsigned counterBits = sizeof(Identifier) * 8 - indexBits;
  static_assert((sizeof(Identifier) * 8) == (indexBits + counterBits),
                "indexBits + counterBits has to be the same as the Identifier width!");

public:
  Entity() = default;
  explicit Entity(Identifier identifier);
  Entity(Identifier index, Identifier counter);

  Index index() const;
  Counter counter() const;

  bool valid() const;

private:
private:
  uint32_t mId{invalidId};
};

inline Entity::Entity(Identifier identifier) : mId(identifier) {}
inline Entity::Entity(Identifier index, Identifier counter) : Entity(index | (counter << indexBits)) {}

inline auto Entity::index() const -> Index
{
  return (mId << counterBits) >> counterBits;
} // namespace aw::ecs

inline auto Entity::counter() const -> Counter
{
  return mId >> indexBits;
}

inline bool Entity::valid() const
{
  return mId != invalidId;
}

inline bool operator==(const ecs::Entity& e1, const ecs::Entity& e2)
{
  return e1.index() == e2.index() && e1.counter() == e2.counter();
}

inline bool operator!=(const ecs::Entity& e1, const ecs::Entity& e2)
{
  return !(e1 == e2);
}
} // namespace aw::ecs
