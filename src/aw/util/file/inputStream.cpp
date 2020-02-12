#include "SDL_rwops.h"

#include <aw/util/file/inputStream.hpp>

namespace aw::file {
InputStream::InputStream(const char* path) : std::istream(nullptr), mBuffer(std::make_unique<SDLStreamBuffer>(path))
{
  // Handle android asset file differently because they are zipped inside the apk
  if (mBuffer && mBuffer->isOpen()) {
    rdbuf(mBuffer.get());
    return;
  } else {
    mBuffer.reset();
  }
}

SDLStreamBuffer::SDLStreamBuffer(const std::string& filePath) : mHandle{SDL_RWFromFile(filePath.c_str(), "rb")}
{
  if (mHandle) {
    mBuffer.resize(1024);

    setg(nullptr, nullptr, nullptr);
    setp(&mBuffer.front(), &mBuffer.front() + mBuffer.size());
  }
}

SDLStreamBuffer::~SDLStreamBuffer()
{
  if (mHandle) {
    sync();
    SDL_RWclose(mHandle);
  }
}

std::streambuf::int_type SDLStreamBuffer::underflow()
{
  auto counter = SDL_RWread(mHandle, mBuffer.data(), sizeof(mBuffer[0]), mBuffer.size());

  if (counter <= 0) {
    return traits_type::eof();
  }

  setg(mBuffer.data(), mBuffer.data(), mBuffer.data() + counter);

  return traits_type::to_int_type(*gptr());
}

std::streambuf::int_type SDLStreamBuffer::overflow(std::streambuf::int_type /*value*/)
{
  return traits_type::eof();
};

auto SDLStreamBuffer::sync() -> int
{
  std::streambuf::int_type result = overflow(traits_type::eof());

  return traits_type::eq_int_type(result, traits_type::eof()) ? -1 : 0;
}

bool SDLStreamBuffer::isOpen() const
{
  return mHandle;
}
} // namespace aw::file
