#pragma once

#include <functional>
#include <iostream>
#include <limits>
#include <vector>

namespace aw::msg {
class ChannelBase
{
public:
  virtual ~ChannelBase() = default;
};

template <typename EventType>
class Channel;

template <typename EventType>
class [[nodiscard]] Subscription
{
public:
  using SubscriptionId = typename Channel<EventType>::SubscriptionId;

public:
  Subscription() = delete;
  Subscription(Channel<EventType> & channel, SubscriptionId subscriptionId);
  ~Subscription();

  Subscription(const Subscription&) = delete;
  auto operator=(const Subscription)->Subscription& = delete;
  Subscription(Subscription && o) noexcept;
  auto operator=(Subscription&& o) noexcept->Subscription&;

  void unsubscribe();

private:
private:
  Channel<EventType>& mChannel;
  SubscriptionId mSubscriptionId;
};

template <typename EventType>
class Channel : public ChannelBase
{
public:
  using Callback = std::function<bool(const EventType&)>;
  using SubscriptionId = unsigned;
  constexpr static SubscriptionId npos = std::numeric_limits<unsigned>::max();

public:
  auto broadcast(const EventType& event) const -> bool;

  auto subscribe(Callback callback) -> Subscription<EventType>;
  [[nodiscard]] auto subscribeUnsafe(Callback callback) -> SubscriptionId;
  auto unsubscribe(SubscriptionId id) -> bool;

private:
private:
  SubscriptionId mIdCounter{0};
  std::vector<SubscriptionId> mIdMappings;
  std::vector<Callback> mSubscribers;
};
} // namespace aw::msg

// Implementation
namespace aw::msg {
template <typename EventType>
Subscription<EventType>::Subscription(Channel<EventType>& channel, SubscriptionId subscriptionId) :
    mChannel(channel), mSubscriptionId(subscriptionId)
{}

template <typename EventType>
Subscription<EventType>::~Subscription()
{
  unsubscribe();
}

template <typename EventType>
Subscription<EventType>::Subscription(Subscription&& o) noexcept :
    mChannel(o.mChannel), mSubscriptionId(o.mSubscriptionId)
{
  o.mSubscriptionId = Channel<EventType>::npos;
}
template <typename EventType>
auto Subscription<EventType>::operator=(Subscription<EventType>&& o) noexcept -> Subscription<EventType>&
{
  mSubscriptionId = o.mSubscriptionId;
  o.mSubscriptionId = Channel<EventType>::npos;
  return *this;
}

template <typename EventType>
void Subscription<EventType>::unsubscribe()
{
  if (mSubscriptionId != Channel<EventType>::npos) {
    mChannel.unsubscribe(mSubscriptionId);
  }
}

template <typename EventType>
auto Channel<EventType>::broadcast(const EventType& event) const -> bool
{
  for (auto& sub : mSubscribers) {
    if (sub(event)) {
      return true;
    }
  }
  return false;
}

template <typename EventType>
auto Channel<EventType>::subscribe(Callback callback) -> Subscription<EventType>
{
  return {*this, subscribeUnsafe(std::move(callback))};
}

template <typename EventType>
auto Channel<EventType>::subscribeUnsafe(Callback callback) -> SubscriptionId
{
  auto id = mIdCounter++;
  mIdMappings.push_back(id);
  mSubscribers.push_back(std::move(callback));

  return id;
}

template <typename EventType>
auto Channel<EventType>::unsubscribe(SubscriptionId id) -> bool
{
  for (unsigned i = 0; i < mIdMappings.size(); i++) {
    if (mIdMappings[i] == id) {
      mIdMappings.erase(std::next(mIdMappings.begin(), i));
      mSubscribers.erase(std::next(mSubscribers.begin(), i));

      return true;
    }
  }
  return false;
}

} // namespace aw::msg
