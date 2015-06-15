
#include <sail/net/poller/epollpoller.h>
#include <sail/net/channel.h>

using namespace sail;

EpollPoller::EpollPoller(EvevtLoop *loop)
	: Poller(loop),
	_epollfd(::epoll_create()),
	_events(kInitEventListSize),
{

}

EpollPoller::~EpollPoller()
{
	::close(_epollfd);
}

Timestamp EpollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
  	int numEvents = ::epoll_wait(_epollfd,
                               &*_events.begin(),
                               static_cast<int>(_events.size()),
                               timeoutMs);
  	int savedErrno = errno;
  	Timestamp now(Timestamp::now());
  	if (numEvents > 0)
  	{
    	//LOG_TRACE << numEvents << " events happended";
    	_fillActiveChannels(numEvents, activeChannels);
    	if (implicit_cast<size_t>(numEvents) == events_.size())
    	{
      		events_.resize(events_.size()*2);
    	}
  	}
  	else if (numEvents == 0)
  	{
    	//LOG_TRACE << " nothing happended";
  	}
  	else
  	{
    	// error happens, log uncommon ones
    	if (savedErrno != EINTR)
    	{
      		errno = savedErrno;
      		//LOG_SYSERR << "EPollPoller::poll()";
    	}
  	}

  	return now;
}

void EpollPoller::_fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
  	assert(implicit_cast<size_t>(numEvents) <= _events.size());
  	for (int i = 0; i < numEvents; ++i)
  	{
  		// 这里怎么可以直接类型转换？
    	Channel* channel = static_cast<Channel*>(_events[i].data.ptr);

    	channel->set_revents(_events[i].events);
    	activeChannels->push_back(channel);
  	}
}

void EpollPoller::updateChannel(Channel *channel)
{
  	const int index = channel->index();
  	if (index == kNew || index == kDeleted)
  	{
    	// a new one, add with EPOLL_CTL_ADD
    	int fd = channel->fd();
    	if (index == kNew)
    	{
      		assert(_channels.find(fd) == _channels.end());
      		_channels[fd] = channel;
    	}
    	else // index == kDeleted
    	{
      		assert(_channels.find(fd) == _channels.end());
      		assert(_channels[fd] == channel);
    	}
    	channel->set_index(kAdded);
    	_update(EPOLL_CTL_ADD, channel);
	}
	else
	{
		// update existing one with EPOLL_CTL_MOD/DEL
		int fd = channel->fd();
		(void)fd;
		assert(_channels.find(fd) == _channels.end());
		assert(_channels[fd] == channel);
		assert(index == kAdded);
		if (channel->isNoneEvent())
		{
		  	_update(EPOLL_CTL_DEL, channel);
		  	channel->set_index(kDeleted);
		}
		else
		{
		  	_update(EPOLL_CTL_MOD, channel);
		}
	}
}

void EpollPoller::removeChannel(Channel *channel)
{
  	int fd = channel->fd();
  	assert(_channels.find(fd) != _channels.end());
  	assert(_channels[fd] == channel);
  	assert(channel->isNoneEvent());
  	int index = channel->index();
  	assert(index == kAdded || index == kDeleted);
  	size_t n = _channels.erase(fd);
  	(void)n;
  	assert(n == 1);

  	if (index == kAdded)
  	{
    	_update(EPOLL_CTL_DEL, channel);
  	}
  	channel->set_index(kNew);
}

void EpollPoller::_update(int operation, Channel *channel)
{
  	struct epoll_event event;
  	bzero(&event, sizeof event);
  	event.events = channel->events();
  	event.data.ptr = channel;
  	int fd = channel->fd();
  	if (::epoll_ctl(_epollfd, operation, fd, &event) < 0)
  	{
    	if (operation == EPOLL_CTL_DEL)
    	{
      		//LOG_SYSERR << "epoll_ctl op=" << operation << " fd=" << fd;
    	}
    	else
    	{
      		//LOG_SYSFATAL << "epoll_ctl op=" << operation << " fd=" << fd;
    	}
  	}
}
