#include "SDL.h"
#include "SDL_events.h"
#include "SDL_video.h"
#include "aw/util/log.hpp"

#include <aw/engine/window.hpp>
#include <aw/engine/windowSettings.hpp>
#include <aw/util/messageBus/messageBus.hpp>

namespace aw {
Window::Window(const WindowSettings& settings, const msg::Bus& bus) : mBus{bus}
{
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0) {
    APP_ERROR("sdl init error: {}", SDL_GetError());
    return;
  }

  auto windowParameter = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
#ifdef __ANDROID__
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE | SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  windowParameter |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // OpenGL core profile
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);                          // OpenGL 3+
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);                          // OpenGL 3.3
#endif

  mWindow =
      SDL_CreateWindow(settings.title.c_str(), 0, 0, settings.resolution.x, settings.resolution.y, windowParameter);

  mContext = SDL_GL_CreateContext(mWindow);
  int major{0};
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
  int minor{0};
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &major);
  APP_INFO("OpenGL Version: {}.{}", major, minor);
  SDL_GL_SetSwapInterval(1);
  /* sf::ContextSettings cSettings; */
  /* cSettings.attributeFlags = sf::ContextSettings::Core | sf::ContextSettings::Debug; */
  /* cSettings.majorVersion = 4; */
  /* cSettings.minorVersion = 2; */
  /* cSettings.depthBits = 8; */
}

Window::~Window()
{
  close();
}

void Window::close()
{
  if (mContext) {
    SDL_GL_DeleteContext(mContext);
    mContext = nullptr;
  }
  if (mWindow) {
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
  }
}

void Window::update()
{
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    mBus.channel<SDL_Event>().broadcast(event);
  }

#ifdef __ANDROID__
  static bool first = true;
  if (!first) {
    return;
  }
  first = false;

  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);

  event.window.type = SDL_WINDOWEVENT;
  event.window.event = SDL_WINDOWEVENT_SIZE_CHANGED;
  event.window.data1 = displayMode.w;
  event.window.data2 = displayMode.h;
  mBus.channel<SDL_Event>().broadcast(event);
#endif
}
void Window::display()
{
  SDL_GL_SwapWindow(mWindow);
}

auto Window::open() const -> bool
{
#ifdef __ANDROID__
  return true;
#else
  return mWindow != nullptr;
#endif
}

auto Window::size() const -> aw::Vec2i
{
#ifndef __ANDROID__
  aw::Vec2i size;
  SDL_GetWindowSize(mWindow, &size.x, &size.y);
  ENGINE_DEBUG("Window size: {} {}", size.x, size.y);
  return size;
#else
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  ENGINE_DEBUG("Window size: {} {}", displayMode.w, displayMode.h);
  return {displayMode.w, displayMode.h};
#endif
}

} // namespace aw
