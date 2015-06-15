
#ifndef _SAIL_EPOLL_EPOLLER_H
#define _SAIL_EPOLL_EPOLLER_H

#include <sail/net/poller.h>

#include <vector>

struct epoll_event;

namespace sail
{

class EpollPoller : public Poller
{
public:
	EpollPoller(EventLoop *loop);
	virtual ~EpollPoller();

	virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels);
  	virtual void updateChannel(Channel *channel);
  	virtual void removeChannel(Channel *channel);

private:
  	static const int kInitEventListSize = 16;

  	void _fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
  	void _update(int operation, Channel* channel);

  	typedef std::vector<struct epoll_event> EventList;

  	int _epollfd;
  	EventList _events;
};

}

#endif /*_SAIL_EPOLL_EPOLLER_H*/
