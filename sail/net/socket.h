
#ifndef _SAIL_SOCKET_H
#define _SAIL_SOCKET_H

#include <sail/base/noncopyable.h>

namespace sail
{

class InetAddr;

class Socket : public NonCopyable
{
public:
	explicit Socket(int sockfd)
		: _sockfd(sockfd)
	{ }

	~Socket();

	int fd() const { return _sockfd; }

	void bindAddress(const InetAddr &localaddr);

	void listen();

	int accept(InetAddr *peeraddr);

	void shutdownWrite();

	// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
	void setTcpNoDelay(bool on);

	// Enable/disable SO_REUSEADDR
	void setReuseAddr(bool on);

	// Enable/disable SO_REUSEPORT
	void setReusePort(bool on);

	// Enable/disable SO_KEEPALIVE
	void setKeepAlive(bool on);

	int setSendTimeout(long long ms);


private:
	const int _sockfd;
};

}

#endif /*_SAIL_SOCKET_H*/
