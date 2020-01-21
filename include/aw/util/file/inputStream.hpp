#pragma once

#include <string>

#include <fstream>

namespace aw::file {
class InputStream : public std::istream
{
public:
  InputStream(const char* path);
  ~InputStream();

  bool isOpen() const { return mIsOpen; }

private:
private:
  std::ifstream mFileStream;
  bool mIsOpen;
};
} // namespace aw::file
