
#include <sail/net/socketutil.h>

using namespace sail;
using namespace sail::sockets;

int sockets::setNonBlock(int sockfd)
{
	int flags;

	if ((flags = ::fcntl(fd, F_GETFL)) == -1)
		return -1;

	flags |= O_NONBLOCK;
	if (::fcntl(sockfd, F_SETFL, flags) == -1) 
		return -1;

	return 0;
}

int sockets::setCloseOnExec(int sockfd)
{
	int flags;

	if ((flags = ::fcntl(sockfd, F_GETFD)) == -1)
		return -1;

	flags |= FD_CLOEXEC;
	if (::fcntl(sockfd, F_SETFD, flags) == -1)
		return -1;

	return 0;
}

int sockets::createNonblockingTcp()
{
	int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) return -1;

	if (setNonBlock(sockfd) == -1) return -1;
	if (setCloseOnExec(sockfd) == -1) return -1;

	return sockfd;
}

int sockets::tcpConnect(int sockfd, const struct sockaddr_in &addr)
{
	return ::connect(sockfd, (struct sockaddr *)&addr, sizeof addr);
}

int sockets::bind(int sockfd, const struct sockaddr_in &addr)
{
	return ::bind(sockfd, (struct sockaddr *)&addr, sizeof addr);
}

void sockets::listen(int sockfd)
{
	::listen(sockfd, SOMAXCONN);
}

int sockets::accept(int sockfd, struct sockaddr_in *addr)
{
	socklen_t addrlen = sizeof(*addr);
	int connfd = ::accept(sockfd, (struct sockaddr *)addr, &addrlen);
	if (connfd >= 0) {
		setNonBlock(connfd);
		setCloseOnExec(connfd);
	}

	return connfd;
}

ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
	return ::read(sockfd, buf, count);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
	return ::write(sockfd, buf, count);
}

void sockets::close(int sockfd)
{
	::close(sockfd);
}

void sockets::shutdownWrite(int sockfd)
{
	::shutdown(sockfd, SHUT_WR);
}

int sockets::getSocketError(int sockfd)
{
	int optval;
	socklen_t optlen = sizeof(optval);

	if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) 
	{
 		return errno;
  	}
  	else 
  	{
    	return optval;
  	}
}

void sockets::toIpPort(char *buf, size_t size, const struct sockaddr_in &addr)
{
	assert(size >= INET_ADDRSTRLEN);
  	::inet_ntop(AF_INET, &addr.sin_addr, buf, size);
  	size_t end = ::strlen(buf);
  	uint16_t port = sockets::networkToHost16(addr.sin_port);
  	assert(size > end);
  	snprintf(buf + end, size - end, ":%u", port);
}

void sockets::toIp(char *buf, size_t size, const struct sockaddr_in &addr)
{
  	assert(size >= INET_ADDRSTRLEN);
  	::inet_ntop(AF_INET, &addr.sin_addr, buf, size);
}

void sockets::fromIpPort(const char *ip, uint16_t port, struct sockaddr_in *addr)
{
  	addr->sin_family = AF_INET;
  	addr->sin_port = hostToNetwork16(port);
  	::inet_pton(AF_INET, ip, &addr->sin_addr);
}



