#pragma once

#include "aw/util/spatial/bBox.hpp"
#include "aw/util/spatial/intersectors/bBox.hpp"

#include <functional>
#include <memory>

namespace aw {
template <typename ChildType, typename Intersector>
class Octree
{
public:
  Octree(aw::BBox bBox, size_t maxElementsPerCell = 10, size_t maxDepth = 5);

  Octree(const Octree& o);
  auto operator=(Octree o) -> Octree&;
  Octree(Octree&& o) noexcept;

  void addElement(ChildType element, Intersector intersector);

  auto getBounds() const -> aw::BBox { return mBounds; }

  void traverseElements(const std::function<void(ChildType)>& callback, aw::BBox boundsToCheck,
                        Intersector intersector = Intersector());
  void traverseNodes(const std::function<void(Octree<ChildType, Intersector>&)>& callback, aw::BBox boundsToCheck);

  inline friend void swap(Octree& first, Octree& second)
  {
    using std::swap;

    swap(first.mBounds, second.mBounds);
    swap(first.mMaxElements, second.mMaxElements);
    swap(first.mMaxDepth, second.mMaxDepth);
    swap(first.mElements, second.mElements);
    swap(first.mSplit, second.mSplit);
    swap(first.mChildNodes, second.mChildNodes);
  }

private:
  void split(Intersector intersector);

private:
  aw::BBox mBounds;
  size_t mMaxElements{};
  size_t mMaxDepth{};

  std::vector<ChildType> mElements;

  bool mSplit{false};
  std::array<std::unique_ptr<Octree>, 8> mChildNodes{nullptr};
};
} // namespace aw

namespace aw {
template <typename ChildType, typename Intersector>
Octree<ChildType, Intersector>::Octree(aw::BBox bBox, size_t maxElementsPerCell, size_t maxDepth) :
    mBounds(bBox), mMaxElements(maxElementsPerCell), mMaxDepth(maxDepth)
{}

template <typename ChildType, typename Intersector>
Octree<ChildType, Intersector>::Octree(const Octree& o) :
    mBounds{o.mBounds},
    mMaxElements{o.mMaxElements},
    mMaxDepth{o.mMaxDepth},
    mElements{o.mElements},
    mSplit{o.mSplit},
    mChildNodes{o.mChildNodes}
{}

template <typename ChildType, typename Intersector>
auto Octree<ChildType, Intersector>::operator=(Octree o) -> Octree&
{
  swap(*this, o);
  return *this;
}

template <typename ChildType, typename Intersector>
Octree<ChildType, Intersector>::Octree(Octree&& o) noexcept : Octree(aw::BBox{})
{
  swap(*this, o);
}

template <typename ChildType, typename Intersector>
void Octree<ChildType, Intersector>::addElement(ChildType element, Intersector intersector)
{
  assert(intersector(mBounds, element));

  // Insert directly
  if (!mSplit && (mMaxDepth == 0 || mElements.size() < mMaxElements)) {
    mElements.push_back(element);
    return;
  }

  // We need to split or just test against already present childs
  if (!mSplit) {
    split(intersector);
  }

  // Check for every child
  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      for (int z = 0; z < 2; z++) {
        int index = (x << 2) | (y << 1) | z;
        if (intersector(mChildNodes[index]->mBounds, element)) {
          mChildNodes[index]->addElement(element, intersector);
        }
      }
    }
  }
}

template <typename ChildType, typename Intersector>
void Octree<ChildType, Intersector>::split(Intersector intersector)
{
  auto center = mBounds.center();
  auto extend = mBounds.extend();
  // Create the 8 childs
  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      for (int z = 0; z < 2; z++) {
        int index = (x << 2) | (y << 1) | z;
        Vec3 offset{static_cast<float>(x) - 0.5f, static_cast<float>(y) - 0.5f, static_cast<float>(z) - 0.5f};
        Vec3 childCenter = center + extend * offset;
        Vec3 childExtend = extend * 0.5f;
        aw::BBox childBounds{childCenter, childExtend};

        mChildNodes[index] = std::make_unique<Octree<ChildType, Intersector>>(childBounds, mMaxElements, mMaxDepth - 1);
      }
    }
  }
  mSplit = true;
  // Add existing elements again (this will insert them into the new childs)
  for (auto& it : mElements) {
    addElement(it, intersector);
  }
  // The nodes are not stored in the childs
  std::vector<ChildType>().swap(mElements);
}

template <typename ChildType, typename Intersector>
void Octree<ChildType, Intersector>::traverseElements(const std::function<void(ChildType)>& callback,
                                                      aw::BBox boundsToCheck, Intersector intersector)
{
  if (!BBoxBBoxIntersector()(mBounds, boundsToCheck)) {
    return;
  }

  if (mSplit) {
    for (auto& child : mChildNodes) {
      child->traverseElements(callback, boundsToCheck);
    }
  } else {
    for (auto element : mElements) {
      if (intersector(boundsToCheck, element)) {
        callback(element);
      }
    }
  }
}
template <typename ChildType, typename Intersector>
void Octree<ChildType, Intersector>::traverseNodes(const std::function<void(Octree<ChildType, Intersector>&)>& callback,
                                                   aw::BBox boundsToCheck)
{
  if (!BBoxBBoxIntersector()(mBounds, boundsToCheck)) {
    return;
  }
  callback(*this);
  if (mSplit) {
    for (auto& child : mChildNodes) {
      child->traverseNodes(callback, boundsToCheck);
    }
  }
}

} // namespace aw
