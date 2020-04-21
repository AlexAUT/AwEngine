#pragma once

#include "aw/config.hpp"
#include "aw/engine/stateMachine.hpp"
#include "aw/engine/window.hpp"
#include "aw/util/filesystem/pathRegistry.hpp"
#include "aw/util/messageBus/messageBus.hpp"

namespace aw {
class AW_API_EXPORT Engine
{
public:
  Engine(int argc, char** argv, const std::string& appName);

  void run();

  auto stateMachine() -> StateMachine&;
  auto stateMachine() const -> const StateMachine&;

  auto messageBus() -> msg::Bus&;
  auto messageBus() const -> const msg::Bus&;

  auto window() -> Window&;
  auto window() const -> const Window&;

  auto pathRegistry() -> PathRegistry&;
  auto pathRegistry() const -> const PathRegistry&;

  auto shouldTerminate() const -> bool;
  void shouldTerminate(bool value);

  void pause(bool value);
  auto pause() const -> bool;

private:
  PathRegistry mPathRegistry;

  bool mLoggerInitialized;

  msg::Bus mBus;

  Window mWindow;
  StateMachine mStateMachine;

  bool mShouldTerminate{false};

  bool mPause{false};
};
} // namespace aw
