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
  msg::Bus& messageBus();

  bool shouldTerminate() const;
  void shouldTerminate(bool value);

  void pause(bool value);
  auto pause() const -> bool;

private:
  msg::Bus mBus;

  Window mWindow;
  StateMachine mStateMachine;

  bool mShouldTerminate{false};

  bool mPause{false};
};
} // namespace aw
