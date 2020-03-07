#pragma once

#include "aw/util/messageBus/channel.hpp"
#include "aw/util/messageBus/messageBus.hpp"

#include <iostream>

namespace aw::msg {
class Bus;

// Utility class to subscribe/unsibscribe automatically
template <typename HostClass, typename... EventTypes>
class Subscriber
{
public:
  Subscriber(msg::Bus& bus) :
      mSubscriptions({(bus.channel<EventTypes>().subscribe([this](const auto& event) -> bool {
        auto* instance = static_cast<HostClass*>(this);
        // Allow instance->receive to return void and just return false (backwards compability)
        if constexpr (std::is_void_v<decltype(instance->receive(event))>) {
          instance->receive(event);
          return false;
        } else {
          return instance->receive(event);
        }
      }))...})
  {}

private:
private:
  std::tuple<typename msg::Subscription<EventTypes>...> mSubscriptions;
};
} // namespace aw::msg
