#include "aw/util/filesystem/fileStream.hpp"

#include "SDL_rwops.h"

namespace aw {

SDLStreamBuffer::SDLStreamBuffer(const aw::fs::path& filePath, const char* flags) :
    mHandle{SDL_RWFromFile(filePath.c_str(), flags)}
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

auto SDLStreamBuffer::underflow() -> std::streambuf::int_type
{
  auto counter = SDL_RWread(mHandle, mBuffer.data(), sizeof(mBuffer[0]), mBuffer.size());

  if (counter <= 0) {
    return traits_type::eof();
  }

  setg(mBuffer.data(), mBuffer.data(), mBuffer.data() + counter);

  return traits_type::to_int_type(*gptr());
}

auto SDLStreamBuffer::overflow(std::streambuf::int_type value) -> std::streambuf::int_type
{
  xsputn(pbase(), pptr() - pbase());
  setp(pbase(), epptr());

  return value;
}

auto SDLStreamBuffer::sync() -> int
{
  std::streambuf::int_type result = overflow(traits_type::eof());
  return traits_type::eq_int_type(result, traits_type::eof()) ? -1 : 0;
}

auto SDLStreamBuffer::xsputn(const char* s, std::streamsize n) -> std::streamsize
{
  SDL_RWwrite(mHandle, s, n, 1);
  return n;
}

auto SDLStreamBuffer::isOpen() const -> bool
{
  return mHandle;
}

FileInputStream::FileInputStream(fs::path fileName) :
    std::istream(nullptr), mPath(std::move(fileName)), mBuffer(std::make_unique<SDLStreamBuffer>(mPath.c_str(), "rb"))
{
  // Handle android asset file differently because they are zipped inside the apk
  if (mBuffer && mBuffer->isOpen()) {
    rdbuf(mBuffer.get());
    return;
  }
  mBuffer.reset();
}

auto FileInputStream::isOpen() const -> bool
{
  return mBuffer != nullptr && mBuffer->isOpen();
}

auto FileInputStream::path() const -> const fs::path&
{
  return mPath;
}

FileOutputStream::FileOutputStream(fs::path fileName) :
    std::ostream(nullptr), mPath(std::move(fileName)), mBuffer(mPath.c_str(), "wb")
{
  // Handle android asset file differently because they are zipped inside the apk
  if (mBuffer.isOpen()) {
    rdbuf(&mBuffer);
    return;
  }
}

auto FileOutputStream::isOpen() const -> bool
{
  return mBuffer.isOpen();
}

auto FileOutputStream::path() const -> const fs::path&
{
  return mPath;
}
} // namespace aw
