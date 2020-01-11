#include <aw/engine/window.hpp>

#include <aw/engine/windowSettings.hpp>
#include <aw/util/message/bus.hpp>

#include <SFML/Window/Event.hpp>

namespace aw {
Window::Window(const WindowSettings& settings, const msg::Bus& bus) : mBus{bus}
{
  sf::ContextSettings cSettings;
  cSettings.attributeFlags = sf::ContextSettings::Core | sf::ContextSettings::Debug;
  cSettings.majorVersion = 4;
  cSettings.minorVersion = 2;

  mWindow.create({settings.resolution.x, settings.resolution.y}, settings.title, sf::Style::Default, cSettings);
}

void Window::close()
{
  mWindow.close();
}

void Window::update()
{
  sf::Event event;
  while (mWindow.pollEvent(event)) {
    mBus.channel<sf::Event>().broadcast(event);
  }
}
void Window::display()
{
  mWindow.display();
}

bool Window::open() const
{
  return mWindow.isOpen();
}

} // namespace aw
