
#include <sail/net/poller.h>
#include <sail/net/poller/poll.h>
#include <sail/net/poller/epoll.h>

#include <stdlib.h>

using namespace sail;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
	if (::getenv("MUDUO_USE_POLL"))
  	{
    	return new PollPoller(loop);
  	}
  	else
  	{
    	return new EPollPoller(loop);
  	}
}