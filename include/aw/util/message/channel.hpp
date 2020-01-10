#pragma once

#include <functional>
#include <iostream>
#include <limits>
#include <vector>

namespace aw::msg
{
class ChannelBase
{
public:
  virtual ~ChannelBase() {}
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
  Subscription& operator=(const Subscription) = delete;
  Subscription(Subscription && o);
  Subscription& operator=(Subscription&& o);

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
  using Callback = std::function<void(const EventType&)>;
  using SubscriptionId = unsigned;
  constexpr static SubscriptionId npos = std::numeric_limits<unsigned>::max();

public:
  void broadcast(const EventType& event) const;

  Subscription<EventType> subscribe(Callback callback);
  [[nodiscard]] SubscriptionId subscribeUnsafe(Callback callback);
  bool unsubscribe(SubscriptionId id);

private:
private:
  SubscriptionId mIdCounter{0};
  std::vector<SubscriptionId> mIdMappings;
  std::vector<Callback> mSubscribers;
};
} // namespace aw::msg

// Implementation
namespace aw::msg
{
template <typename EventType>
Subscription<EventType>::Subscription(Channel<EventType>& channel, SubscriptionId subscriptionId) :
    mChannel(channel), mSubscriptionId(subscriptionId)
{
}

template <typename EventType>
Subscription<EventType>::~Subscription()
{
  unsubscribe();
}

template <typename EventType>
Subscription<EventType>::Subscription(Subscription&& o) : mChannel(o.mChannel), mSubscriptionId(o.mSubscriptionId)
{
  o.mSubscriptionId = Channel<EventType>::npos;
}
template <typename EventType>
Subscription<EventType>& Subscription<EventType>::operator=(Subscription<EventType>&& o)
{
  mSubscriptionId = o.mSubscriptionId;
  o.mSubscriptionId = Channel<EventType>::npos;
  return Subscription(o);
}

template <typename EventType>
void Subscription<EventType>::unsubscribe()
{
  if (mSubscriptionId != Channel<EventType>::npos)
  {
    mChannel.unsubscribe(mSubscriptionId);
  }
}

template <typename EventType>
void Channel<EventType>::broadcast(const EventType& event) const
{
  for (auto& sub : mSubscribers)
    sub(event);
}

template <typename EventType>
Subscription<EventType> Channel<EventType>::subscribe(Callback callback)
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
bool Channel<EventType>::unsubscribe(SubscriptionId id)
{
  for (unsigned i = 0; i < mIdMappings.size(); i++)
  {
    if (mIdMappings[i] == id)
    {
      mIdMappings.erase(std::next(mIdMappings.begin(), i));
      mSubscribers.erase(std::next(mSubscribers.begin(), i));

      return true;
    }
  }
  return false;
}

} // namespace aw::msg
