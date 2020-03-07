#pragma once

#include "aw/config.hpp"
#include "aw/ecs/componentStorage.hpp"
#include "aw/util/type/staticFor.hpp"

namespace aw::ecs {
template <typename Component>
struct Optional
{
  using value_type = Component;
};
template <typename T>
struct isOptional : std::false_type
{};
template <typename T>
struct isOptional<Optional<T>> : std::true_type
{};
template <typename T>
inline constexpr bool isOptional_v = std::is_base_of_v<std::true_type, isOptional<T>>;

template <typename T>
struct removeOptional
{
  using type = T;
};
template <typename T>
struct removeOptional<Optional<T>>
{
  using type = T;
};
template <typename T>
using removeOptional_t = typename removeOptional<T>::type;

template <typename... Components>
class View
{
public:
  using Storages = std::tuple<ComponentStorage<removeOptional_t<Components>>&...>;

public:
  class Iterator
  {
  public:
    using TupleComponents = std::tuple<Components...>;
    using EntityIterator = typename ComponentStorage<std::decay_t<std::tuple_element_t<0, Storages>>>::EntityIterator;

  public:
    Iterator(EntityIterator entityIterator, Storages& storages) : mEntityIterator{entityIterator}, mStorages{storages}
    {}

    operator bool() const { return static_cast<bool>(mEntityIterator); }

    auto operator==(const Iterator& it) const -> bool { return (mEntityIterator == it.mEntityIterator); }
    auto operator!=(const Iterator& it) const -> bool { return (mEntityIterator != it.mEntityIterator); }

    auto valid() -> bool
    {
      bool haveAll = true;
      aw::staticFor<sizeof...(Components)>([this, &haveAll](auto i) {
        if (isOptional_v<std::tuple_element_t<i, TupleComponents>> || !haveAll) {
          return;
        }
        auto e = *mEntityIterator;
        auto& storage = std::get<i>(mStorages);
        haveAll = storage.has(e);
      });
      return haveAll;
    }

    auto operator++() -> Iterator&
    {
      auto end = std::get<0>(mStorages).entityEnd();
      do {
        ++mEntityIterator;
      } while (mEntityIterator != end && !valid());

      return (*this);
    }
    auto operator++(int) -> Iterator
    {
      auto temp(*this);
      ++(*this);
      return temp;
    }

    template <size_t... Is>
    auto getAll(std::index_sequence<Is...>)
    {
      auto e = *mEntityIterator;
      auto get = [this, e](auto i) {
        if constexpr (isOptional_v<std::tuple_element_t<i, TupleComponents>>) {
          auto& s = std::get<i>(mStorages);
          return std::tuple(s.get(e));
        } else {
          auto& s = std::get<i>(mStorages);
          return std::tie(*s.get(e));
        }
      };

      return std::tuple_cat(std::make_tuple(e), get(std::integral_constant<std::size_t, Is>{})...);
      //(f(std::integral_constant<std::size_t, Is>{}), ...);
      /* return std::apply( */
      /*     [&](auto&... s) { */
      /*       auto e = *mEntityIterator; */
      /*       return std::make_tuple(e, get(s, Is)...); */
      /*     }, */
      /*     mStorages); */
    }

    auto operator*()
    {
      using seq = std::make_integer_sequence<std::size_t, sizeof...(Components)>;
      return getAll(seq{});

      /* return std::apply( */
      /*     [&](auto&... s) { */
      /*       auto e = *mEntityIterator; */
      /*       return std::make_tuple(e, s.get(e)...); */
      /*     }, */
      /*     mStorages); */
    }

    auto operator-> () -> auto* { return *this; }

  private:
    EntityIterator mEntityIterator;
    Storages& mStorages;
  };

public:
  View() = default;
  template <typename WorldT>
  explicit View(WorldT& world) : mStorages{world.template getStorage<removeOptional_t<Components>>()...}
  {}

  View(const View&) = delete;
  View(View&&) = delete;
  auto operator=(const View&) -> View& = delete;
  auto operator=(View &&) -> View& = delete;

  auto begin()
  {
    auto& storage = std::get<0>(mStorages);
    if (storage.entityBegin() == storage.entityEnd()) {
      return end();
    }
    auto beg = Iterator(storage.entityBegin(), mStorages);
    if (!beg.valid()) {
      ++beg;
    }
    return beg;
  }
  auto end() { return Iterator(std::get<0>(mStorages).entityEnd(), mStorages); }

private:
private:
  Storages mStorages;
}; // namespace aw::ecs
} // namespace aw::ecs
