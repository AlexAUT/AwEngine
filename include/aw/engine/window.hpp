#pragma once

#include "SDL_video.h"
#include "aw/config.hpp"
#include "aw/util/math/vector.hpp"

#ifdef AW_OS_ANDROID
#include "SDL_egl.h"
#endif

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

  auto handle() -> SDL_Window*;
  auto handle() const -> const SDL_Window*;

  auto context() -> SDL_GLContext&;
  auto context() const -> const SDL_GLContext&;

private:
private:
  SDL_Window* mWindow;
  SDL_GLContext mContext;
  const msg::Bus& mBus;

#ifdef AW_OS_ANDROID
  EGLSurface mSurface;
  EGLDisplay mDisplay;
#endif
};
} // namespace aw
