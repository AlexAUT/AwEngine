#pragma once

#include <aw/util/message/channel.hpp>
#include <aw/util/type/typeCounter.hpp>

#include <memory>
#include <vector>

namespace aw::msg
{
class Bus
{
public:
  Bus() = default;
  Bus(const Bus&) = delete;
  Bus(const Bus&&) = delete;
  Bus& operator=(const Bus&) = delete;
  Bus& operator=(const Bus&&) = delete;

  template <typename EventType>
  Channel<EventType>& channel();

  template <typename EventType>
  const Channel<EventType>& channel() const;

private:
private:
  using TypeCounter = aw::type::Counter<Bus>;
  mutable std::vector<std::unique_ptr<ChannelBase>> mChannels;
};
} // namespace aw::msg

// Implementation
namespace aw::msg
{
template <typename EventType>
Channel<EventType>& Bus::channel()
{
  auto channelIndex = TypeCounter::id<EventType>();

  if (mChannels.size() <= channelIndex)
    mChannels.resize(channelIndex + 1);

  if (!mChannels[channelIndex])
  {
    mChannels[channelIndex].reset(new Channel<EventType>());
  }
  return *static_cast<Channel<EventType>*>(mChannels[channelIndex].get());
}

template <typename EventType>
const Channel<EventType>& Bus::channel() const
{
  auto channelIndex = TypeCounter::id<EventType>();

  if (mChannels.size() <= channelIndex)
    mChannels.resize(channelIndex + 1);

  if (!mChannels[channelIndex])
  {
    mChannels[channelIndex].reset(new Channel<EventType>());
  }
  return *static_cast<Channel<EventType>*>(mChannels[channelIndex].get());
}
} // namespace aw::msg
