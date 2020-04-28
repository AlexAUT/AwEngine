#include "aw/util/filesystem/pathRegistry.hpp"

#ifndef AW_OS_ANDROID
#include "platform_folders.h"
#else
#include "SDL.h"
#endif

#include <aw/config.hpp>

#include <cassert>

namespace aw {

PathRegistry::PathRegistry(int argc, const char* const* argv, const std::string& appName)
{
  // We are not allowed to call LOG functions here!
  assert(argc > 0);

  mWorkingPath = fs::current_path();

  mExecutablePath = fs::path(argv[0]).parent_path();

#ifndef AW_OS_ANDROID
  mAssetPath = mWorkingPath / "assets";
#else
  mAssetPath = "";
#endif

#ifndef AW_OS_ANDROID
  mConfigPath = fs::path(sago::getConfigHome()).append(appName);
#else
  mConfigPath = SDL_AndroidGetInternalStoragePath();
  mConfigPath = mConfigPath / "config";
#endif
  fs::create_directories(mConfigPath);

#ifndef AW_OS_ANDROID
  mLogPath = fs::path(sago::getCacheDir()).append(appName);
#else
  mLogPath = SDL_AndroidGetInternalStoragePath();
  mLogPath = mLogPath / "log";
#endif
  fs::create_directories(mLogPath);
}

auto PathRegistry::workingPath() const -> const fs::path&
{
  return mWorkingPath;
}

auto PathRegistry::executablePath() const -> const fs::path&
{
  return mExecutablePath;
}

auto PathRegistry::assetPath() const -> const fs::path&
{
  return mAssetPath;
}

auto PathRegistry::configPath() const -> const fs::path&
{
  return mConfigPath;
}

auto PathRegistry::logPath() const -> const fs::path&
{
  return mLogPath;
}
} // namespace aw
