#pragma once

#define ForwardGPUHandle(

namespace aw {
template <typename Type>
struct GPUHandle
{
  GPUHandle() = default;
  explicit GPUHandle(Type handle) : handle{handle} {}

  GPUHandle(const GPUHandle&&) = delete;
  GPUHandle& operator=(const GPUHandle&&) = delete;

  GPUHandle(GPUHandle&& o) : handle(o.handle) { o.handle = 0; }
  GPUHandle& operator=(GPUHandle&& o)
  {
    handle = o.handle;
    o.handle = 0;
  }

  Type handle{0};
};
} // namespace aw
