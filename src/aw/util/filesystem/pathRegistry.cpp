#include "aw/util/filesystem/pathRegistry.hpp"

#include "platform_folders.h"

#include <cassert>

namespace aw {

PathRegistry::PathRegistry(int argc, const char* const* argv, const std::string& appName)
{
  // We are not allowed to call LOG functions here!
  assert(argc > 0);

  mWorkingPath = fs::current_path();

  mExecutablePath = fs::path(argv[0]).parent_path();

  mAssetPath = mWorkingPath / "assets";

  mConfigPath = fs::path(sago::getConfigHome()).append(appName);
  fs::create_directories(mConfigPath);

  mLogPath = fs::path(sago::getCacheDir()).append(appName);
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
