
#ifndef _SAIL_CHANNEL_H
#define _SAIL_CHANNEL_H

#include <sail/base/noncopyable.h>
#include <sail/base/timestamp.h>

namespace sail
{

class EventLoop;

///
/// A selectable I/O channel.
///
/// This class doesn't own the file descriptor.
/// The file descriptor could be a socket,
/// an eventfd, a timerfd, or a signalfd
class Channel : public NonCopyable
{
public:
	typedef void EventCallback();
	typedef int ReadEventCallback(Timestamp stamp);

	Channel(EventLoop *loop, int fd);
	~Channel();

	void setReadCallback(ReadEventCallback * cb)
  	{ _readCallback = cb; }
  	void setWriteCallback(EventCallback *cb)
  	{ _writeCallback = cb; }
  	void setCloseCallback(EventCallback *cb)
  	{ _closeCallback = cb; }
  	void setErrorCallback(EventCallback *cb)
  	{ _errorCallback = cb; }


  	int fd() const { return _fd; }
	int events() const { return _events; }
	void set_revents(int revt) { _revents = revt; } // used by pollers

	bool isNoneEvent() const { return _events == kNoneEvent; }

	void enableReading() { _events |= kReadEvent; _update(); }
	void disableReading() { _events &= ~kReadEvent; _update(); }
	void enableWriting() { _events |= kWriteEvent; _update(); }
	void disableWriting() { _events &= ~kWriteEvent; _update(); }
	void disableAll() { _events = kNoneEvent; _update(); }
	bool isWriting() const { return _events & kWriteEvent; }

	void remove();
	void handleEvent(Timestamp receiveTime);

private:
    void _update();

private:
	static const int kNoneEvent;
  	static const int kReadEvent;
  	static const int kWriteEvent;

	EventLoop* _loop;
	const int  _fd;
	int        _events;
	int        _revents; // it's the received event types of epoll or poll
	int        _index; // used by Poller.

	ReadEventCallback *_readCallback;
	EventCallback *_writeCallback;
	EventCallback *_closeCallback;
	EventCallback *_errorCallback;

};

}

#endif /*_SAIL_CHANNEL_H*/
