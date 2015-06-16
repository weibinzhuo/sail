
#include <sail/net/channel.h>
#include <sail/net/eventloop.h>

#include <poll.h>
#include <stdlib.h>

using namespace sail;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd)
  : _loop(loop),
	_fd(fd),
	_events(0),
	_revents(0),
	_index(-1),
	_readCallback(NULL),
	_writeCallback(NULL),
	_closeCallback(NULL),
	_errorCallback(NULL)
{
}

Channel::~Channel()
{
}

void Channel::remove()
{
	_loop->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
	if ((_revents & POLLHUP) && !(_revents & POLLIN))
	{
		if (_closeCallback) _closeCallback();
	}

	if (_revents & (POLLERR | POLLNVAL))
	{
		if (_errorCallback) _errorCallback();
	}
	if (_revents & (POLLIN | POLLPRI | POLLRDHUP))
	{
		if (_readCallback) _readCallback(receiveTime);
	}
	if (_revents & POLLOUT)
	{
		if (_writeCallback) _writeCallback();
	}

}

