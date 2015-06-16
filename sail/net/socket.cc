
#include <sail/net/socket.h>
#include <sail/net/inetaddr.h>
#include <sail/net/socketutil.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>  // bzero

using namespace sail;

Socket::~Socket()
{
	sockets::close(_sockfd);
}

void Socket::bindAddress(const InetAddr &localaddr)
{
	sockets::bind(_sockfd, localaddr.getSockAddrInet());
}

void Socket::listen()
{
	sockets::listen(_sockfd);
}

int Socket::accept(InetAddr *peeraddr)
{
	struct sockaddr_in addr;
	bzero(&addr, sizeof addr);
  	int connfd = sockets::accept(_sockfd, &addr);
  	if (connfd >= 0)
  	{
  		peeraddr->setSockAddrInet(addr);
  	}
  	return connfd;
}


void Socket::shutdownWrite()
{
  sockets::shutdownWrite(_sockfd);
}

void Socket::setTcpNoDelay(bool on)
{
	int optval = on ? 1: 0;
	::setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY,
               	&optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on)
{
 	int optval = on ? 1 : 0;
  	::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR,
               	&optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on)
{
	int optval = on ? 1 : 0;
	int ret = ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEPORT,
				&optval, static_cast<socklen_t>(sizeof optval));
	if (ret < 0){
		//print error
	}
}

void Socket::setKeepAlive(bool on)
{
	int optval = on ? 1 : 0;
  	::setsockopt(_sockfd, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
}

int Socket::setSendTimeout(long long ms)
{
    struct timeval tv;
    tv.tv_sec = ms/1000;
    tv.tv_usec = (ms%1000)*1000;

    if (::setsockopt(_sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == -1) {
        //anetSetError(err, "setsockopt SO_SNDTIMEO: %s", strerror(errno));
        return -1;
    }
    return 0;
}


