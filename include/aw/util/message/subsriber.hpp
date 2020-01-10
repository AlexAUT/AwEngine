#pragma once

#include <aw/util/message/bus.hpp>
#include <aw/util/message/channel.hpp>

#include <iostream>

namespace aw::msg
{
class Bus;

// Utility class to subscribe/unsibscribe automatically
template <typename HostClass, typename... EventTypes>
class Subscriber
{
public:
  Subscriber(msg::Bus& bus) :
      mSubscriptions({(bus.channel<EventTypes>().subscribe([this](const auto& event) {
        auto* instance = static_cast<HostClass*>(this);
        instance->receive(event);
      }))...})
  {
  }

private:
private:
  std::tuple<typename msg::Subscription<EventTypes>...> mSubscriptions;
};
} // namespace aw::msg
