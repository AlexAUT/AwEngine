#pragma once

#include "aw/config.hpp"
#include "aw/engine/state.hpp"
#include "aw/util/time/time.hpp"

#include <cstddef>
#include <memory>
#include <queue>
#include <stack>

namespace aw {
// Simple StateMachine with on active state
class AW_API_EXPORT StateMachine
{
public:
  auto activeState() -> State*;

  // This does not switch directly, the switch will always happen after the update function is called
  void nextState(std::unique_ptr<State> state);

  // Replaced the active state with the next state if set
  void update();

private:
  std::unique_ptr<State> mActiveState{nullptr};
  std::unique_ptr<State> mNextState{nullptr};
};
} // namespace aw
