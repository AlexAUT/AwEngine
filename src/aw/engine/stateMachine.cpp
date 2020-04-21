
#include <aw/engine/stateMachine.hpp>

namespace aw {
auto StateMachine::activeState() -> State*
{
  return mActiveState.get();
}

void StateMachine::nextState(std::unique_ptr<State> state)
{
  mNextState = std::move(state);
}

void StateMachine::update()
{
  if (mNextState) {
    mActiveState = std::move(mNextState);
  }
}

} // namespace aw

