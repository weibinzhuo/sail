
#include <sail/net/poller.h>
#include <sail/net/channel.h>

using namespace sail;

Poller::Poller(EventLoop* loop)
  : _loop(loop)
{
}

Poller::~Poller()
{
}

bool Poller::hasChannel(Channel *channel) const
{
	ChannelMap::const_iterator it = _channels.find(channel->fd());
	return it != _channels.end() && it->second == channel;
}
