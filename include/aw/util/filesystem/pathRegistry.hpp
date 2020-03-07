#pragma once

#include "aw/config.hpp"
#include "aw/util/filesystem/filesystem.hpp"

namespace aw {
class AW_API_EXPORT PathRegistry
{
public:
  PathRegistry(int argc, const char* const* argv, const std::string& appName);

  auto workingPath() const -> const fs::path&;
  auto assetPath() const -> const fs::path&;
  auto configPath() const -> const fs::path&;
  auto logPath() const -> const fs::path&;

private:
private:
  fs::path mWorkingPath;
  fs::path mAssetPath;
  fs::path mConfigPath;
  fs::path mLogPath;
};
} // namespace aw
