#include <aw/engine/engine.hpp>

#include <aw/engine/windowSettings.hpp>
#include <aw/util/log.hpp>
#include <aw/util/time/clock.hpp>

#include <SFML/Window/Event.hpp>

namespace aw {
Engine::Engine(int argc, const char** argv) : mWindow{WindowSettings{"AwEngine", aw::Vec2u{1280, 720}}, mBus}
{
  auto temp = mBus.channel<sf::Event>().subscribeUnsafe([this](auto& e) {
    if (e.type == sf::Event::Closed) {
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
  const aw::Seconds updateRate{1.f / 2.f};

  while (activeState && mWindow.open()) {
    mWindow.update();

    frameTime += frameClock.restart();
    while (frameTime >= updateRate) {
      activeState->update(updateRate);
      frameTime -= updateRate;
    }

    activeState->render();
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

bool Engine::shouldTerminate() const
{
  return mShouldTerminate;
}

void Engine::shouldTerminate(bool value)
{
  mShouldTerminate = value;
}
} // namespace aw
