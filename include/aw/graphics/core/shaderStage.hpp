#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include <vulkan/vk_safe_struct.h>

namespace vk
{

}

namespace aw
{
class ShaderStage
{
public:
  enum class Type
  {
    Vertex,
    Fragment
  };

  ShaderStage(const ShaderStage&) = delete;
  ShaderStage& operator=(const ShaderStage&) = delete;
  ShaderStage(ShaderStage&&) = default;
  ShaderStage& operator=(ShaderStage&) = delete;

private:
  vk::ShaderModule module;
};
} // namespace aw
