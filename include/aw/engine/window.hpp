#pragma once

#include "SDL_video.h"
#include "aw/config.hpp"
#include "aw/util/math/vector.hpp"

namespace aw {
struct WindowSettings;

namespace msg {
class Bus;
}

class AW_API_EXPORT Window
{
public:
  explicit Window(const WindowSettings& settings, const msg::Bus& messageBus);
  ~Window();

  void close();

  void update();
  void display();

  auto open() const -> bool;

  auto size() const -> aw::Vec2i;

private:
private:
  SDL_Window* mWindow;
  SDL_GLContext mContext;
  const msg::Bus& mBus;
};
} // namespace aw
