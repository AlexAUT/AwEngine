#include <aw/util/file/inputStream.hpp>

namespace aw::file {
InputStream::InputStream(const char* path) : std::istream(nullptr)
{
  // Handle android asset file differently because they are zipped inside the apk
#ifdef AW_OS_ANDROID
  if (aw::path::isAsset(mPath)) {
    auto* buffer = new AndroidAssetBuffer(mPath);
    mIsOpen = buffer->isOpen();
    if (mIsOpen)
      rdbuf(buffer);
    else
      delete buffer;
    return;
  }
#endif
  mFileStream.open(path, std::ios::binary);
  mIsOpen = mFileStream.is_open();
  rdbuf(mFileStream.rdbuf());
}

InputStream::~InputStream()
{
#ifdef AW_OS_ANDROID
  if (mIsAssetFile) {
    delete rdbuf();
    return;
  }
#endif
  rdbuf(nullptr);
  mFileStream.close();
}

} // namespace aw::file
