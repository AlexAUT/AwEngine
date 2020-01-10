#include <aw/engine/state.hpp>

namespace aw
{
State::State(StateMachine& stateMachine) : mStateMachine(stateMachine) {}

StateMachine& State::stateMachine()
{
  return mStateMachine;
}
} // namespace aw
