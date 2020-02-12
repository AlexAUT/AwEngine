#pragma once

#include "SDL_rwops.h"

#include <fstream>
#include <istream>
#include <memory>
#include <string>
#include <vector>

namespace aw::file {

class SDLStreamBuffer : public std::streambuf
{
public:
  explicit SDLStreamBuffer(const std::string& fileName);
  ~SDLStreamBuffer();

  auto underflow() -> std::streambuf::int_type override;
  auto overflow(std::streambuf::int_type value) -> std::streambuf::int_type override;
  auto sync() -> int override;

  auto isOpen() const -> bool;

private:
  SDL_RWops* mHandle{nullptr};
  std::vector<char> mBuffer;
};

class InputStream : public std::istream
{
public:
  explicit InputStream(const char* path);

  auto isOpen() const -> bool { return mBuffer != nullptr; }

private:
private:
  std::unique_ptr<SDLStreamBuffer> mBuffer;
};
} // namespace aw::file
