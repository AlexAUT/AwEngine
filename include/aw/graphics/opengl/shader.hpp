#pragma once

#include "aw/config.hpp"
#include "aw/util/filesystem/filesystem.hpp"

namespace aw::gl {

namespace shaderStage {

enum class AW_API_EXPORT Type
{
  Vertex,
  Fragment
};

auto AW_API_EXPORT createFromFile(const fs::path& path, Type type) -> unsigned;
} // namespace shaderStage

namespace shader {
auto AW_API_EXPORT createProgram(unsigned vertexShader, unsigned fragmentShader) -> unsigned;

}
} // namespace aw::gl
