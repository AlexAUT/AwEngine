#pragma once

#include <SFML/Window/Window.hpp>

namespace aw {
struct WindowSettings;

namespace msg {
class Bus;
}

class Window
{
public:
  explicit Window(const WindowSettings& settings, const msg::Bus& messageBus);

  void close();

  void update();
  void display();

  bool open() const;

private:
private:
  sf::Window mWindow;
  const msg::Bus& mBus;
};
} // namespace aw
