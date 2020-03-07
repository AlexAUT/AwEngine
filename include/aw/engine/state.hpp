#pragma once

#include "aw/config.hpp"

#include <aw/util/time/time.hpp>

namespace aw {
class StateMachine;

class AW_API_EXPORT State
{
public:
  explicit State(StateMachine& stateMachine);
  virtual ~State() = default;

  auto stateMachine() -> StateMachine&;

  virtual void update(aw::Seconds dt) = 0;
  virtual void render() = 0;

private:
  StateMachine& mStateMachine;
};
} // namespace aw
