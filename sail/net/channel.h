
#ifndef _SAIL_CHANNEL_H
#define _SAIL_CHANNEL_H

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

	void setReadCallback(const ReadEventCallback *cb)
  	{ _readCallback = cb; }
  	void setWriteCallback(const EventCallback *cb)
  	{ _writeCallback = cb; }
  	void setCloseCallback(const EventCallback *cb)
  	{ _closeCallback = cb; }
  	void setErrorCallback(const EventCallback *cb)
  	{ _errorCallback = cb; }


  	int fd() const { return _fd; }
	int events() const { return _events; }
	void set_revents(int revt) { _revents = revt; } // used by pollers

	bool isNoneEvent() const { return _events == kNoneEvent; }

	void enableReading() { _events |= kReadEvent; update(); }
	void disableReading() { _events &= ~kReadEvent; update(); }
	void enableWriting() { _events |= kWriteEvent; update(); }
	void disableWriting() { _events &= ~kWriteEvent; update(); }
	void disableAll() { _events = kNoneEvent; update(); }
	bool isWriting() const { return _events & kWriteEvent; }

	void remove();
	void handleEvent(Timestamp receiveTime);

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