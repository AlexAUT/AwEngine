#include "aw/graphics/opengl/shader.hpp"

#include "aw/graphics/opengl/gl.hpp"
#include "aw/util/filesystem/convert.hpp"
#include "aw/util/filesystem/fileStream.hpp"
#include "aw/util/log.hpp"

#include <array>

namespace aw::gl {

namespace shaderStage {

auto toGL(Type type) -> GLenum
{
  switch (type) {
  case Type::Vertex:
    return GL_VERTEX_SHADER;
  case Type::Fragment:
    return GL_FRAGMENT_SHADER;
  default:
    return GL_NONE;
  }
}

auto createFromFile(const fs::path& path, Type type) -> unsigned
{
  aw::FileInputStream file(path);
  if (!file.isOpen()) {
    APP_ERROR("Could not open shader file: {}", path.c_str());
    return 0;
  }

  auto content = aw::toString(file);
  const char* contentPtr = content.data();
  const char** contentIndirect = &contentPtr;
  (void)contentIndirect;

  auto stage = glCreateShader(toGL(type));

#ifdef __ANDROID__
  const char* shaderVersion;
  if (type == Type::Fragment) {
    shaderVersion = "#version 300 es\nprecision mediump float;\n";
  } else {
    shaderVersion = "#version 300 es\n";
  }
#else
  const char* shaderVersion = "#version 430\n";
#endif

  std::array<const char*, 2> shaderSource{shaderVersion, content.data()};

  glShaderSource(stage, 2, &shaderSource[0], nullptr);
  glCompileShader(stage);

  int success;
  glGetShaderiv(stage, GL_COMPILE_STATUS, &success);
  if (!success) {
    int size = 0;
    glGetShaderiv(stage, GL_INFO_LOG_LENGTH, &size);
    std::vector<char> info(static_cast<std::size_t>(size));
    info.resize(size + 1U);
    glGetShaderInfoLog(stage, static_cast<GLsizei>(info.size()), nullptr, info.data());

    APP_ERROR("Failed to compile shader stage {}: {}", path.c_str(), info.data());

    glDeleteShader(stage);
    return 0;
  }

  return stage;
}
} // namespace shaderStage

namespace shader {
auto createProgram(unsigned vertexShader, unsigned fragmentShader) -> unsigned
{
  auto prog = glCreateProgram();
  glAttachShader(prog, vertexShader);
  glAttachShader(prog, fragmentShader);

  glLinkProgram(prog);

  int success;
  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if (!success) {
    int size = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &size);
    std::vector<char> info(size + 1U);
    glGetProgramInfoLog(prog, static_cast<GLsizei>(info.size()), nullptr, info.data());

    APP_ERROR("Failed to link shader program: {}", info.data());

    glDeleteProgram(prog);
    return 0;
  }

  return prog;
}
} // namespace shader
} // namespace aw::gl
