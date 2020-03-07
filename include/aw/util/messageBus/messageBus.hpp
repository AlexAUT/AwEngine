#pragma once

#include "aw/util/messageBus/channel.hpp"
#include "aw/util/type/typeCounter.hpp"

#include <memory>
#include <vector>

namespace aw::msg {
class Bus
{
public:
  Bus() = default;
  Bus(const Bus&) = delete;
  Bus(const Bus&&) = delete;
  auto operator=(const Bus&) -> Bus& = delete;
  auto operator=(const Bus &&) -> Bus& = delete;

  template <typename EventType>
  auto channel() -> Channel<EventType>&;

  template <typename EventType>
  auto channel() const -> const Channel<EventType>&;

private:
private:
  using TypeCounter = aw::type::Counter<Bus>;
  mutable std::vector<std::unique_ptr<ChannelBase>> mChannels;
};
} // namespace aw::msg

// Implementation
namespace aw::msg {
template <typename EventType>
auto Bus::channel() -> Channel<EventType>&
{
  auto channelIndex = TypeCounter::id<EventType>();

  if (mChannels.size() <= channelIndex) {
    mChannels.resize(channelIndex + 1);
  }

  if (!mChannels[channelIndex]) {
    mChannels[channelIndex].reset(new Channel<EventType>());
  }
  return *static_cast<Channel<EventType>*>(mChannels[channelIndex].get());
}

template <typename EventType>
auto Bus::channel() const -> const Channel<EventType>&
{
  auto channelIndex = TypeCounter::id<EventType>();

  if (mChannels.size() <= channelIndex) {
    mChannels.resize(channelIndex + 1);
  }

  if (!mChannels[channelIndex]) {
    mChannels[channelIndex].reset(new Channel<EventType>());
  }
  return *static_cast<Channel<EventType>*>(mChannels[channelIndex].get());
}
} // namespace aw::msg
