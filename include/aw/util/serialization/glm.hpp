#pragma once

#include "glm/vec2.hpp"

template <class Archive>
void serialize(Archive& archive, glm::uvec2& vec)
{
  archive(vec.x, vec.y);
}

