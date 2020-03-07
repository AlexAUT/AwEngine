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
void file(const fs::path& path, Type& instance)
{
  aw::FileOutputStream outFile(path);
  cereal::JSONOutputArchive archive(outFile);
  archive(instance);
}
} // namespace aw::serialize

namespace aw::parse {
template <typename Type>
void file(const fs::path& path, Type& instance)
{
  aw::FileInputStream inFile(path);
  if (!inFile.isOpen()) {
    ENGINE_WARN("Could not open file to parse: {}", path.c_str());
    return;
  }
  cereal::JSONInputArchive archive(inFile);
  archive(instance);
}

template <typename Type>
auto file(const fs::path& path) -> Type
{
  Type instance;
  file(path, instance);
  return instance;
}

} // namespace aw::parse
