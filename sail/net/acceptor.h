
#ifndef _SAIL_ACCEPTOR_H
#define _SAIL_ACCEPTOR_H

#include <sail/base/noncopyable.h>
#include <sail/net/channel.h>
#include <sail/net/socket.h>

namespace sail
{

class EventLoop;
class InetAddr;

class Acceptor : public NonCopyable
{
public:
	typedef void NewConnCallback(int sockfd, const InetAddr&);

	Acceptor(EventLoop* loop, const InetAddr &listenAddr, bool reuseport);
    ~Acceptor();

  	void setNewConnectionCallback(NewConnCallback *cb)
  	{ _newConnectionCallback = cb; }

  	bool listenning() const { return _listenning; }
  	void listen();

private:
  	void _handleRead();

	EventLoop *_loop;
  	Socket _acceptSocket;
  	Channel _acceptChannel;
  	NewConnCallback *_newConnectionCallback;
  	bool _listenning;
  	int _idleFd;
};

}

#endif /*_SAIL_ACCEPTOR_H*/


