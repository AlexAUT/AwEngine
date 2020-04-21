#include "aw/engine/particleSystem/renderer.hpp"

#include "aw/graphics/opengl/error.hpp"
#include "aw/graphics/opengl/gl.hpp"
#include "aw/graphics/opengl/shader.hpp"
#include "aw/util/color/colors.hpp"
#include "aw/util/filesystem/pathRegistry.hpp"
#include "aw/util/log.hpp"
#include "aw/util/time/time.hpp"

// float angle = 0.f;
// float angleDiff = aw::pi() / resolutionH;
//
// std::array<aw::Vec2, resolution + 2> vertexData;
// vertexData[0] = aw::Vec2{0.f};
// for (int x = 0; x < resolutionH; x++) {
// vertexData[1 + x] = aw::Vec2{glm::cos(angle), glm::sin(angle)};
// angle += angleDiff;
//}
// angle = aw::pi();
// for (int x = resolutionH; x >= 0; x--) {
// vertexData[1 + resolutionH + (resolutionH - x)] = aw::Vec2{glm::cos(angle), -glm::sin(angle)};
// angle -= angleDiff;
//}

namespace aw {
ParticleRenderer::ParticleRenderer(const aw::PathRegistry& pathRegistry, aw::Vec2i windowSize)
{
  GL_CHECK(glGenBuffers(1, &mVertexVbo));
  GL_CHECK(glGenBuffers(1, &mParticleVbo));

  //  std::array<aw::Vec2, 4> vertices{aw::Vec2{-0.5f, -0.5f}, aw::Vec2{0.5f, -0.5f}, aw::Vec2{0.5f, 0.5f},
  //                                   aw::Vec2{-0.5f, 0.5f}};
  std::array<aw::Vec2, 4> vertices{aw::Vec2{-0.5f, 0.5f}, aw::Vec2{-0.5f, -0.5f}, aw::Vec2{0.5f, -0.5f},
                                   aw::Vec2{0.5f, 0.5f}};

  GL_CHECK(glGenVertexArrays(1, &mVao));
  GL_CHECK(glBindVertexArray(mVao));

  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVertexVbo));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.front()) * vertices.size(), vertices.data(), GL_STATIC_DRAW));
  GL_CHECK(glEnableVertexAttribArray(0));
  GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr));

  using Particle = aw::ParticleSystem::Particle;
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mParticleVbo));
  // TTL/Position3D
  GL_CHECK(glEnableVertexAttribArray(1));
  GL_CHECK(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), nullptr));
  GL_CHECK(glVertexAttribDivisor(1, 1));
  // size1D
  GL_CHECK(glEnableVertexAttribArray(2));
  GL_CHECK(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle),
                                 reinterpret_cast<const void*>(offsetof(Particle, velocity))));
  GL_CHECK(glVertexAttribDivisor(2, 1));

  GL_CHECK(glEnableVertexAttribArray(3));
  GL_CHECK(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
                                 reinterpret_cast<const void*>(offsetof(Particle, rotation))));
  GL_CHECK(glVertexAttribDivisor(3, 1));

  GL_CHECK(glBindVertexArray(0));

  using Type = aw::gl::shaderStage::Type;
  auto vShader = aw::gl::shaderStage::createFromFile(pathRegistry.assetPath() / "shaders/particle.vert", Type::Vertex);
  auto fShader =
      aw::gl::shaderStage::createFromFile(pathRegistry.assetPath() / "shaders/particle.frag", Type::Fragment);

  mProgram = aw::gl::shader::createProgram(vShader, fShader);

  GL_CHECK(glDeleteShader(fShader));
  GL_CHECK(glDeleteShader(vShader));

  GL_CHECK(glUseProgram(mProgram));
  mVpLoc = glGetUniformLocation(mProgram, "viewProjection");
  mSimTimeLoc = glGetUniformLocation(mProgram, "simulationTime");
  mGradientTextureLoc = glGetUniformLocation(mProgram, "colorGradient");
  GL_CHECK(glUniform1i(mGradientTextureLoc, 0));

  GL_CHECK(glUseProgram(0));

  // Generate color gradient texture
  GL_CHECK(glGenTextures(1, &mColorGradientTexture));

  windowResize(windowSize);
}

ParticleRenderer::~ParticleRenderer()
{
  GL_CHECK(glDeleteTextures(1, &mColorGradientTexture));
  GL_CHECK(glDeleteProgram(mProgram));
  GL_CHECK(glDeleteVertexArrays(1, &mVao));

  GL_CHECK(glDeleteBuffers(1, &mParticleVbo));
  GL_CHECK(glDeleteBuffers(1, &mVertexVbo));
}

void ParticleRenderer::render(const aw::Mat4& vp, aw::Seconds simulationTime,
                              const std::vector<aw::ParticleSystem::ParticleContainer>& particles)
{
  GL_CHECK(glDisable(GL_DEPTH_TEST));
  GL_CHECK(glEnable(GL_BLEND));
  GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE));

  GL_CHECK(glUseProgram(mProgram));
  GL_CHECK(glUniformMatrix4fv(mVpLoc, 1, GL_FALSE, &vp[0][0]));
  GL_CHECK(glUniform1f(mSimTimeLoc, simulationTime.count()));

  GL_CHECK(glActiveTexture(GL_TEXTURE0));
  GL_CHECK(glBindTexture(GL_TEXTURE_1D, mColorGradientTexture));

  GL_CHECK(glBindVertexArray(mVao));

  for (const auto& pLayer : particles) {
    colorGradient(pLayer.colorGradient, pLayer.fadeInTime);

    const auto& p = pLayer.particles;
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mParticleVbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(p.front()) * p.size(), p.data(), GL_STREAM_DRAW));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GL_CHECK(glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, static_cast<GLsizei>(p.size())));
  }

  GL_CHECK(glBindVertexArray(0));
}

void ParticleRenderer::windowResize(aw::Vec2i windowSize)
{
  float aspectRation = static_cast<float>(windowSize.y) / static_cast<float>(windowSize.x);
  GL_CHECK(glUseProgram(mProgram));
  GL_CHECK(glUniform1f(glGetUniformLocation(mProgram, "aspectRatio"), aspectRation));
  GL_CHECK(glUseProgram(0));
}

auto lerp(float f, float a, float b) -> float
{
  return a + f * (b - a);
}

auto mix(float ratio, Color a, Color b) -> aw::Color
{
  return aw::Color{lerp(ratio, a.r, b.r), lerp(ratio, a.g, b.g), lerp(ratio, a.b, b.b), lerp(ratio, a.a, b.a)};
}

void ParticleRenderer::colorGradient(Gradient gradient, float fadeIn)
{
  if (mFadeIn == fadeIn && mColorGradient == gradient) {
    return;
  }

  mFadeIn = fadeIn;
  mColorGradient = gradient;

  GL_CHECK(glBindTexture(GL_TEXTURE_1D, mColorGradientTexture));

  constexpr int size = 128;
  int startUp = static_cast<int>(size * fadeIn);
  int gradientSize = size - startUp;

  std::array<aw::Color, size> finalGradient;
  for (int i = 0; i < startUp; i++) {
    finalGradient[i] = mix(static_cast<float>(i) / static_cast<float>(startUp), aw::Colors::Transparent, gradient[0]);
  }
  for (int i = 0; i < gradientSize; i++) {
    finalGradient[i + startUp] =
        mix(static_cast<float>(i) / static_cast<float>(gradientSize), gradient[0], gradient[1]);
  }

  GL_CHECK(glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, finalGradient.size(), 0, GL_RGBA, GL_FLOAT, finalGradient.data()));

  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
} // namespace aw
