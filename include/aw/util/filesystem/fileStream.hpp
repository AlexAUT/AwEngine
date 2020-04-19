#pragma once

#include "SDL_rwops.h"
#include "aw/config.hpp"
#include "aw/util/filesystem/filesystem.hpp"

#include <fstream>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace aw {

class AW_API_EXPORT SDLStreamBuffer : public std::streambuf
{
public:
  explicit SDLStreamBuffer(const fs::path& filePath, const char* flags);
  ~SDLStreamBuffer() override;

  auto underflow() -> std::streambuf::int_type override;
  auto overflow(std::streambuf::int_type value) -> std::streambuf::int_type override;
  auto sync() -> int override;

  auto xsputn(const char* s, std::streamsize n) -> std::streamsize override;

  [[nodiscard]] auto isOpen() const -> bool;

private:
  SDL_RWops* mHandle{nullptr};
  std::vector<char> mBuffer;
};

class AW_API_EXPORT FileInputStream : public std::istream
{
public:
  explicit FileInputStream(fs::path filePath);

  auto isOpen() const -> bool;
  auto path() const -> const fs::path&;

private:
private:
  fs::path mPath;
  std::unique_ptr<SDLStreamBuffer> mBuffer;
};

class AW_API_EXPORT FileOutputStream : public std::ostream
{
public:
  explicit FileOutputStream(fs::path filePath);

  auto isOpen() const -> bool;
  auto path() const -> const fs::path&;

private:
  fs::path mPath;
  SDLStreamBuffer mBuffer;
};
} // namespace aw
