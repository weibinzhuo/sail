
#ifndef _SAIL_EVENTLOOP_H
#define _SAIL_EVENTLOOP_H

#include <vector>

#include <sail/base/noncopyable.h>
#include <sail/base/mutexlock.h>

namespace sail
{

class Channel;
class Poller;
class TimerQueue;

class EventLoop : public NonCopyable
{
public:
	EventLoop();
	~EventLoop();

	void loop();

	void quit();

    void removeChannel(Channel *channel);

private:
	void _processActiveChannels();

private:
	bool _quit;
	bool _eventHandling;

	typedef std::vector<Channel*> ChannelList;
	ChannelList _activeChannels;

	Poller* _poller;
	MutexLock _mutex;
};

}

#endif /*_SAIL_EVENTLOOP_H*/


