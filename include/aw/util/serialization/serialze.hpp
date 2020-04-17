#pragma once

#include "aw/util/filesystem/fileStream.hpp"
#include "aw/util/filesystem/filesystem.hpp"
#include "aw/util/log.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/xml.hpp"

#include <filesystem>
#include <fstream>

namespace aw::serialize {

template <typename Type>
void file(const fs::path& path, Type& instance, const char* key = nullptr)
{
  aw::FileOutputStream outFile(path);
  {
    cereal::JSONOutputArchive archive(outFile);
    if (key) {
      archive(cereal::make_nvp(key, instance));
    } else {
      archive(instance);
    }
  }
}
} // namespace aw::serialize

namespace aw::parse {
template <typename Type>
void file(const fs::path& path, Type& instance, const char* key = nullptr)
{
  aw::FileInputStream inFile(path);
  if (!inFile.isOpen()) {
    ENGINE_WARN("Could not open file to parse: {}", path.c_str());
    return;
  }
  cereal::JSONInputArchive archive(inFile);
  if (key) {
    archive(cereal::make_nvp(key, instance));
  } else {
    archive(instance);
  }
}

template <typename Type>
auto file(const fs::path& path, const char* key = nullptr) -> Type
{
  Type instance;
  file(path, instance, key);
  return instance;
}

} // namespace aw::parse
