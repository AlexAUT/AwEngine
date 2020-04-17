#pragma once

#include "aw/graphics/opengl/gl.hpp"

namespace gpu::priv {
auto glCheckError(const char* stmt, const char* file, int line) -> GLenum;
} // namespace gpu::priv

#ifdef _DEBUG
#define GL_CHECK(stmt)                                                                                                 \
  do {                                                                                                                 \
    stmt;                                                                                                              \
    gpu::priv::glCheckError(#stmt, __FILE__, __LINE__);                                                                \
  } while (false)
#else
#define GL_CHECK(stmt) stmt
#endif
