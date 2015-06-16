
#ifndef _SAIL_POLLER_H
#define _SAIL_POLLER_H

#include <map>
#include <vector>
#include <sail/base/noncopyable.h>
#include <sail/base/timestamp.h>

namespace sail
{

class Channel;
class EventLoop;

class Poller : public sail::NonCopyable
{
public:
	typedef std::vector<Channel*> ChannelList;

	Poller(EventLoop *loop);
	virtual ~Poller();

	/// Polls the I/O events.
	/// Must be called in the loop thread.
	virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;

	/// Changes the interested I/O events.
	/// Must be called in the loop thread.
	virtual void updateChannel(Channel *channel) = 0;

	/// Remove the channel, when it destructs.
	/// Must be called in the loop thread.
	virtual void removeChannel(Channel *channel) = 0;

	virtual bool hasChannel(Channel *channel) const;

	static Poller* newDefaultPoller(EventLoop *loop);

protected:
	typedef std::map<int, Channel*> ChannelMap;
  	ChannelMap _channels;

private:
	EventLoop *_loop;
};

}

#endif /*_SAIL_POLLER_H*/
