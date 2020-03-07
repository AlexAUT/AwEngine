#include "aw/util/filesystem/convert.hpp"

namespace aw {
auto toString(std::istream& stream, size_t chunkSize) -> std::string
{
  std::string stringBuffer;
  stringBuffer.resize(chunkSize);
  size_t cursor = 0;
  while (stream.read(reinterpret_cast<char*>(stringBuffer.data() + cursor), chunkSize)) {
    cursor += chunkSize;
    stringBuffer.resize(cursor + chunkSize);
  }
  stringBuffer.resize(cursor + stream.gcount());

  return stringBuffer;
}

auto toUint8(std::istream& stream, size_t chunkSize) -> std::vector<uint8_t>
{
  std::vector<uint8_t> buffer(chunkSize);
  size_t cursor = 0;
  while (stream.read(reinterpret_cast<char*>(buffer.data() + cursor), chunkSize)) {
    cursor += chunkSize;
    buffer.resize(cursor + chunkSize);
  }
  buffer.resize(cursor + stream.gcount());

  return buffer;
}
} // namespace aw
