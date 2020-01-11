#pragma once

#include <aw/engine/stateMachine.hpp>
#include <aw/engine/window.hpp>
#include <aw/util/message/bus.hpp>

namespace aw {
class Engine
{
public:
  Engine(int argc, const char** argv);

  void run();

  StateMachine& stateMachine();

  bool shouldTerminate() const;
  void shouldTerminate(bool value);

private:
  msg::Bus mBus;

  Window mWindow;
  StateMachine mStateMachine;

  bool mShouldTerminate{false};
};
} // namespace aw
