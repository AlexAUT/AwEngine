#include "aw/engine/engine.hpp"

#include "SDL_events.h"
#include "aw/engine/performance.hpp"
#include "aw/engine/windowSettings.hpp"
#include "aw/util/log.hpp"
#include "aw/util/time/clock.hpp"

namespace aw {
Engine::Engine(int argc, char** argv, const std::string& appName) :
    mPathRegistry(argc, argv, appName),
    mLoggerInitialized{aw::priv::log::init(appName, mPathRegistry.logPath())},
    // Be careful to not remove the log init call as first thing this constructor does
    mWindow{WindowSettings{appName, aw::Vec2u{1280, 720}}, mBus}
{
  [[maybe_unused]] auto temp = mBus.channel<SDL_Event>().subscribeUnsafe([this](SDL_Event e) {
    if (e.type == SDL_WINDOWEVENT && e.window.type == SDL_WINDOWEVENT_CLOSE) {
      shouldTerminate(true);
    }
    if (e.type == SDL_QUIT) {
      shouldTerminate(true);
    }
    return false;
  });
}

void Engine::run()
{
  // We need to call update here to active the initial state!
  mStateMachine.update();
  auto* activeState = mStateMachine.activeState();

  aw::Clock frameClock;
  aw::Seconds frameTime{0.f};
  const aw::Seconds updateRate{1.f / 60.f};

  aw::perf::Section frameSection("Engine::frameTime");
  aw::perf::Section updateSection("Engine::update");
  aw::perf::Section renderSection("Engine::render");

  while ((activeState != nullptr) && mWindow.open()) {
    frameSection.start();

    updateSection.start();

    if (mPause) {
      mWindow.update();
      activeState->update(aw::Seconds::zero());
      frameClock.restart();
    } else {
      mWindow.update();
      frameTime += frameClock.restart();
      while (frameTime >= updateRate) {
        activeState->update(updateRate);
        frameTime -= updateRate;
      }
    }

    updateSection.end();

    renderSection.start();
    activeState->render();

    renderSection.end();
    frameSection.end();

    mWindow.display();

    // Check for new active state, if so always update the system with 0dt to ensure update is called before the first
    // render
    mStateMachine.update();
    auto* newActiveState = mStateMachine.activeState();
    if (newActiveState != activeState) {
      activeState = newActiveState;
      activeState->update(aw::Seconds{0});
    }

    if (mShouldTerminate) {
      mWindow.close();
    }
  }
}

auto Engine::stateMachine() -> StateMachine&
{
  return mStateMachine;
}

auto Engine::stateMachine() const -> const StateMachine&
{
  return mStateMachine;
}

auto Engine::messageBus() -> msg::Bus&
{
  return mBus;
}

auto Engine::messageBus() const -> const msg::Bus&
{
  return mBus;
}

auto Engine::window() -> Window&
{
  return mWindow;
}

auto Engine::window() const -> const Window&
{
  return mWindow;
}

auto Engine::pathRegistry() -> PathRegistry&
{
  return mPathRegistry;
}

auto Engine::pathRegistry() const -> const PathRegistry&
{
  return mPathRegistry;
}

auto Engine::shouldTerminate() const -> bool
{
  return mShouldTerminate;
}

void Engine::shouldTerminate(bool value)
{
  mShouldTerminate = value;
}

void Engine::pause(bool value)
{
  mPause = value;
}

auto Engine::pause() const -> bool
{
  return mPause;
}
} // namespace aw
