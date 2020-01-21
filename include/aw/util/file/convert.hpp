#pragma once

#include <cstdint>
#include <istream>
#include <string>
#include <vector>

namespace aw::file {
std::string toString(std::istream& stream, size_t chunkSize = 2048);
std::vector<uint8_t> toUint8(std::istream& stream, size_t chunkSize = 2048);
} // namespace aw::file
