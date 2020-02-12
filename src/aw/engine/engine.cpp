#include "SDL_events.h"
#include "aw/engine/performance.hpp"

#include <aw/engine/engine.hpp>
#include <aw/engine/windowSettings.hpp>
#include <aw/util/log.hpp>
#include <aw/util/time/clock.hpp>

namespace aw {
Engine::Engine(int argc, char** argv) :
    mLoggerInitialized{aw::log::priv::init()},
    // Be careful to not remove the log init call as first thing this constructor does
    mWindow{WindowSettings{"AwEngine", aw::Vec2u{1280, 720}}, mBus}
{
  auto temp = mBus.channel<SDL_Event>().subscribeUnsafe([this](SDL_Event e) {
    if (e.type == SDL_WINDOWEVENT && e.window.type == SDL_WINDOWEVENT_CLOSE) {
      shouldTerminate(true);
    }
    if (e.type == SDL_QUIT) {
      shouldTerminate(true);
    }
  });
}

void Engine::run()
{
  mStateMachine.update();
  auto activeState = mStateMachine.activeState();

  aw::Clock frameClock;
  aw::Seconds frameTime{0.f};
  const aw::Seconds updateRate{1.f / 60.f};

  aw::perf::Section frameSection("Engine::frameTime");
  aw::perf::Section updateSection("Engine::update");
  aw::perf::Section renderSection("Engine::render");

  while ((activeState != nullptr) && mWindow.open()) {
    frameSection.start();

    mWindow.update();
    updateSection.start();

    if (mPause) {
      activeState->update(aw::Seconds::zero());
      frameClock.restart();
    } else {
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

    mStateMachine.update();

    if (mShouldTerminate) {
      mWindow.close();
    }
  }
}

StateMachine& Engine::stateMachine()
{
  return mStateMachine;
}

msg::Bus& Engine::messageBus()
{
  return mBus;
}

bool Engine::shouldTerminate() const
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

bool Engine::pause() const
{
  return mPause;
}
} // namespace aw
