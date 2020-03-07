#pragma once

#include "aw/config.hpp"
#include "aw/ecs/entity.hpp"

#include <cassert>
#include <memory>
#include <optional>
#include <vector>

namespace aw::ecs {
constexpr bool isPowerOf2(int v)
{
  return v && ((v & (v - 1)) == 0);
}

template <typename Type, size_t pageSize = 4096>
class AW_API_EXPORT SparseSet
{
public:
  using DataIterator = typename std::vector<Type>::iterator;
  using EntityIterator = typename std::vector<Entity>::iterator;

public:
  static_assert(isPowerOf2, "Pagesize is only allowed to be a power of two!");

  Type& set(Entity e, Type value);
  template <typename... Args>
  Type& emplace(Entity e, Args&&... args);

  bool has(Entity entity);
  Type* get(Entity e);

  void erase(Entity entity);

  size_t size() const { return mData.size(); }
  auto data() const { return mData.data(); }
  auto dataBegin() { return mData.begin(); }
  auto dataEnd() { return mData.end(); }
  auto dataBegin() const { return mData.begin(); }
  auto dataEnd() const { return mData.end(); }

  auto entityBegin() { return mDirectMapping.begin(); }
  auto entityEnd() { return mDirectMapping.end(); }
  auto entityBegin() const { return mDirectMapping.begin(); }
  auto entityEnd() const { return mDirectMapping.end(); }

private:
  constexpr auto mapping(Entity::Index index);

  static constexpr Entity::Index elementsPerPage = pageSize / sizeof(Type*);
  static_assert(isPowerOf2(elementsPerPage), "Elements per page has to be exactly a power of 2");
  static constexpr int offsetMask = elementsPerPage - 1; // Only valid for power of two pageSize

  using Page = std::array<Entity::Index, elementsPerPage>;

  Page& assure(Entity::Index page);

private:
  std::vector<std::unique_ptr<Page>> mIndirectMapping;
  std::vector<Entity> mDirectMapping;

  std::vector<Type> mData;
};
} // namespace aw::ecs

namespace aw::ecs {
template <typename Type, size_t pageSize>
constexpr auto SparseSet<Type, pageSize>::mapping(Entity::Index index)
{
  auto page = index / elementsPerPage;
  auto offset = index & offsetMask;

  return std::make_tuple(page, offset);
}

template <typename Type, size_t pageSize>
Type& SparseSet<Type, pageSize>::set(Entity e, Type value)
{
  assert(!has(e));

  auto [page, offset] = this->mapping(e.index());

  assert(this->assure(page).size() > offset);

  this->assure(page)[offset] = mData.size();
  mData.push_back(value);
  mDirectMapping.emplace_back(e);
  return mData.back();
}

template <typename Type, size_t pageSize>
template <typename... Args>
Type& SparseSet<Type, pageSize>::emplace(Entity e, Args&&... args)
{
  auto [page, offset] = this->mapping(e.index());

  assert(this->assure(page).size() > offset);

  this->assure(page)[offset] = mData.size();
  if constexpr (std::is_aggregate_v<Type>)
    mData.emplace_back(Type{std::forward<Args>(args)...});
  else
    mData.emplace_back(std::forward<Args>(args)...);
  mDirectMapping.emplace_back(e);
  return mData.back();
}

template <typename Type, size_t pageSize>
bool SparseSet<Type, pageSize>::has(Entity e)
{
  auto [page, offset] = this->mapping(e.index());
  if (page >= mIndirectMapping.size())
    return false;
  return mIndirectMapping[page] && (*mIndirectMapping[page])[offset] != Entity::invalidIndex;
}

template <typename Type, size_t pageSize>
Type* SparseSet<Type, pageSize>::get(Entity e)
{
  auto [page, offset] = this->mapping(e.index());
  if (page >= mIndirectMapping.size() || !mIndirectMapping[page] ||
      (*mIndirectMapping[page])[offset] == Entity::invalidIndex)
    return nullptr;
  return mData.data() + ((*mIndirectMapping[page])[offset]);
}

template <typename Type, size_t pageSize>
void SparseSet<Type, pageSize>::erase(Entity e)
{
  auto [page, offset] = this->mapping(e.index());
  if (page >= mIndirectMapping.size() || !mIndirectMapping[page])
    return;

  auto index = (*mIndirectMapping[page])[offset];
  auto entry = mData.data() + index;
  if (entry) {
    if (index != mDirectMapping.size() - 1) {
      auto [toPage, toOffset] = this->mapping(mDirectMapping.back().index());
      std::swap(*entry, mData.back());

      auto directIter = std::next(mDirectMapping.begin(), index);
      std::swap(*directIter, mDirectMapping.back());

      mData.pop_back();
      mDirectMapping.pop_back();
      // Do not reorder the two lines beneath or otherwise when removing the last element will not work correctly!
      (*mIndirectMapping[toPage])[toOffset] = index;
      (*mIndirectMapping[page])[offset] = Entity::invalidIndex;
    } else {
      mData.pop_back();
      mDirectMapping.pop_back();
      (*mIndirectMapping[page])[offset] = Entity::invalidIndex;
    }
  }
}

template <typename Type, size_t pageSize>
auto SparseSet<Type, pageSize>::assure(Entity::Index page) -> Page&
{
  if (page >= mIndirectMapping.size() || !mIndirectMapping[page]) {
    if (page >= mIndirectMapping.size()) {
      mIndirectMapping.resize(page + 1);
    }
    mIndirectMapping[page] = std::make_unique<Page>();
    std::fill(mIndirectMapping[page]->begin(), mIndirectMapping[page]->end(), Entity::invalidIndex);
  }
  assert(mIndirectMapping[page]);
  return *mIndirectMapping[page];
}

} // namespace aw::ecs
