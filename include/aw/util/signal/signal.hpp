#pragma once

#include <functional>
#include <vector>

namespace aw {
template <typename Signature>
class Signal;

template <typename ReturnValue, typename... Args>
class Signal<ReturnValue(Args...)>
{
public:
  using Id = int;
  using Function = std::function<ReturnValue(const Args&...)>;
  using FunctionWithId = std::function<ReturnValue(const Args&..., Id id)>;
  using IdFunctionPair = std::pair<int, Function>;

public:
  [[nodiscard]] auto connect(Function func) -> Id
  {
    Id newId = mListeners.empty() ? 1 : (mListeners.back().first + 1);
    mListeners.emplace_back(newId, std::move(func));
    return newId;
  }

  auto connect(FunctionWithId func) -> Id
  {
    Id newId = mListeners.empty() ? 1 : (mListeners.back().first + 1);
    mListeners.emplace_back(newId, [=](const Args... args) { return func(std::forward<Args>(args)..., newId); });
    return newId;
  }

  void disconnect(Id toRemove) { mToRemove.push_back(toRemove); }

  void operator()(const Args&... args)
  {
    if (!mToRemove.empty()) {
      removeOldListeners();
    }
    for (auto& listener : mListeners) {
      // TODO bug/undefined bahaviour: this could fail if an early callback does disconnect a later one!
      assert(std::find(mToRemove.begin(), mToRemove.end(), listener.first) == mToRemove.end());
      listener.second(args...);
    }
  }

private:
  void removeOldListeners()
  {
    for (auto toRemove : mToRemove) {
      auto found = std::find_if(mListeners.begin(), mListeners.end(), [=](auto& e) { return toRemove == e.first; });
      if (found != mListeners.end()) {
        mListeners.erase(found);
      }
    }
    mToRemove.clear();
  }

private:
  std::vector<IdFunctionPair> mListeners;
  std::vector<Id> mToRemove;
};
} // namespace aw
