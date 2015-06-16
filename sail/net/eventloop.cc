
#include <sail/net/eventloop.h>
#include <sail/net/poller.h>

using namespace sail;

EventLoop::EventLoop()
  : _quit(false),
  	_eventHandling(false),
	_poller(Poller::newDefaultPoller(this))
{

}

EventLoop::~EventLoop()
{

}

void EventLoop::loop()
{
	_quit = false;

	while(!_quit)
	{
		delta = ngx_current_msec;

		unsigned int timeoutMs = findMinTimeoutMs();

		_activeChannels.clear();
	    pollReturnTime_ = poller_->poll(timeoutMs, &_activeChannels);

	    delta = ngx_current_msec - delta;

	    if (_activeChannels.size() > 0) {
	    	_processActiveChannels();
	    }

	    //
	    if (delta) {
	    	processExpireTimers();
	    }
	}
}

void EventLoop::quit()
{
	_quit = true;
}

void EventLoop::_processActiveChannels()
{
    _eventHandling = true;
    for (ChannelList::iterator it = _activeChannels.begin();
        it != _activeChannels.end(); ++it)
    {
    	_currentActiveChannel = *it;
    	_currentActiveChannel->handleEvent(pollReturnTime_);
    }
    _currentActiveChannel = NULL;
    _eventHandling = false;
}


