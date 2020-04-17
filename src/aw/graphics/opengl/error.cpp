#include "error.h"

#include "aw/graphics/opengl/gl.hpp"
#include "aw/util/log.hpp"

#include <string>

namespace gpu::priv {
auto glCheckError(const char* stmt, const char* file, int line) -> GLenum
{
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    std::string message;
    switch (errorCode) {
    case GL_INVALID_ENUM:
      message = "INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      message = "INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      message = "INVALID_OPERATION";
      break;
#ifdef GL_STACK_OVERFLOW
    case GL_STACK_OVERFLOW:
      message = "STACK_OVERFLOW";
      break;
#endif
#ifdef GL_STACK_UNDERFLOW
    case GL_STACK_UNDERFLOW:
      message = "STACK_UNDERFLOW";
      break;
#endif
    case GL_OUT_OF_MEMORY:
      message = "OUT_OF_MEMORY";
      break;
#ifdef GL_INVALID_FRAMEBUFFER_OPERATION
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      message = "INVALID_FRAMEBUFFER_OPERATION";
      break;
#endif
    default:
      message = "UNKNOWN OPENGL ERROR, CODE: " + std::to_string(errorCode);
    }
    ENGINE_ERROR("Opengl: {} at {} in {}:{}", message, stmt, file, line);
  } // namespace gpu::priv
  return GL_NO_ERROR;
}

} // namespace gpu::priv
