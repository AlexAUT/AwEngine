#pragma once

#include "aw/config.hpp"
#include "aw/engine/state.hpp"
#include "aw/util/time/time.hpp"

#include <cstddef>
#include <memory>
#include <queue>
#include <stack>

namespace aw {
class AW_API_EXPORT StateMachine
{
public:
  auto activeState() -> State*;

  void pushState(std::unique_ptr<State> state);

  void popState(int count = 1);
  void popAllStates();

  void update();

private:
  std::stack<std::unique_ptr<State>> mStateStack;
  std::queue<std::unique_ptr<State>> mPushQueue;
  size_t mPopCount{0};
};
} // namespace aw
