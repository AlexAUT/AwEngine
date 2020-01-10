#pragma once

#include <aw/util/time/time.hpp>

namespace aw
{
class StateMachine;

class State
{
public:
  explicit State(StateMachine& stateMachine);
  virtual ~State() = default;

  StateMachine& stateMachine();

  virtual void update(aw::Seconds dt) = 0;
  virtual void render() = 0;

private:
  StateMachine& mStateMachine;
};
} // namespace aw
