
#include <aw/engine/stateMachine.hpp>

namespace aw {
auto StateMachine::activeState() -> State*
{
  return mStateStack.empty() ? nullptr : mStateStack.top().get();
}

void StateMachine::pushState(std::unique_ptr<State> state)
{
  mPushQueue.push(std::move(state));
}

void StateMachine::popState(int count)
{
  mPopCount += count;
}

void StateMachine::popAllStates()
{
  mPopCount = mStateStack.size();
}

void StateMachine::update()
{
  if (mPopCount > 0) {
    if (mPopCount >= mStateStack.size()) {
      mStateStack = {};
      mPopCount = 0;
    } else {
      while (mPopCount > 0) {
        mStateStack.pop();
        mPopCount--;
      }
    }
  }

  while (!mPushQueue.empty()) {
    mStateStack.push(std::move(mPushQueue.front()));
    mPushQueue.pop();
  }
}

} // namespace aw

