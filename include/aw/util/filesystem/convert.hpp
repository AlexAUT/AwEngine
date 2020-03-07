#pragma once

#include "aw/config.hpp"

#include <cstdint>
#include <istream>
#include <string>
#include <vector>

namespace aw {
AW_API_EXPORT auto toString(std::istream& stream, size_t chunkSize = 2048) -> std::string;
AW_API_EXPORT auto toUint8(std::istream& stream, size_t chunkSize = 2048) -> std::vector<uint8_t>;
} // namespace aw
