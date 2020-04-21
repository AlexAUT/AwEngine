#include <aw/engine/state.hpp>

namespace aw {
State::State(StateMachine& stateMachine) : mStateMachine(stateMachine) {}

auto State::stateMachine() -> StateMachine&
{
  return mStateMachine;
}
} // namespace aw
