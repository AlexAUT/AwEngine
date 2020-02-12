#pragma once

#include "SDL_video.h"

namespace aw {
struct WindowSettings;

namespace msg {
class Bus;
}

class Window
{
public:
  explicit Window(const WindowSettings& settings, const msg::Bus& messageBus);
  ~Window();

  void close();

  void update();
  void display();

  bool open() const;

private:
private:
  SDL_Window* mWindow;
  SDL_GLContext mContext;
  const msg::Bus& mBus;
};
} // namespace aw
