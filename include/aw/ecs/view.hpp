#pragma once

#include <aw/ecs/componentStorage.hpp>
#include <aw/ecs/world.hpp>

namespace aw::ecs
{
template <typename... Components>
class View
{
public:
  class Iterator
  {
  public:
    using Storages = std::tuple<ComponentStorage<Components>&...>;
    using EntityIterator = typename ComponentStorage<std::decay_t<std::tuple_element_t<0, Storages>>>::EntityIterator;

  public:
    Iterator(EntityIterator entityIterator, Storages& storages) : mEntityIterator{entityIterator}, mStorages{storages}
    {
    }

    operator bool() const
    {
      if (mEntityIterator)
        return true;
      else
        return false;
    }

    bool operator==(const Iterator& it) const { return (mEntityIterator == it.mEntityIterator); }
    bool operator!=(const Iterator& it) const { return (mEntityIterator != it.mEntityIterator); }

    Iterator& operator++()
    {
      auto haveAll = [this](auto&... storages) {
        auto e = *mEntityIterator;
        return (storages.has(e) && ...);
      };
      auto end = std::get<0>(mStorages).entityEnd();
      do
      {
        ++mEntityIterator;
      } while (mEntityIterator != end && !std::apply(haveAll, mStorages));

      return (*this);
    }
    Iterator& operator--()
    {
      --mEntityIterator;
      return (*this);
    }
    Iterator operator++(int)
    {
      auto temp(*this);
      (*this)++;
      return temp;
    }
    Iterator operator--(int)
    {
      auto temp(*this);
      --mEntityIterator;
      return temp;
    }

    auto operator*()
    {
      return std::apply(
          [&](auto&... s) {
            auto e = *mEntityIterator;
            return std::tuple_cat(std::make_tuple(e), std::tie(*s.get(e)...));
          },
          mStorages);
    }
    const auto operator*() const
    {
      return std::apply(
          [&](auto&... s) {
            auto e = *mEntityIterator;
            return std::tuple_cat(std::make_tuple(e), std::tie(*s.get(e)...));
          },
          mStorages);
    }
    auto* operator-> () { return *this; }

  private:
    EntityIterator mEntityIterator;
    Storages& mStorages;
  };

public:
  View() = default;
  View(World& world) : mStorages{world.getStorage<Components>()...} {}

  auto begin() { return Iterator(std::get<0>(mStorages).entityBegin(), mStorages); }
  auto end() { return Iterator(std::get<0>(mStorages).entityEnd(), mStorages); }

private:
private:
  using Storages = std::tuple<ComponentStorage<Components>&...>;
  Storages mStorages;
};
} // namespace aw::ecs
