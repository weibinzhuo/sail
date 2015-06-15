
#ifndef _SAIL_SOCKET_UTIL_H
#define _SAIL_SOCKET_UTIL_H

#include <arpa/inet.h>

namespace sail
{
namespace sockets
{

	int setNonBlock(int sockfd);
	int setCloseOnExec(int sockfd);

	int createNonblockingTcp();
	int tcpConnect(int sockfd, const struct sockaddr_in &addr);

	void bind(int sockfd, const struct sockaddr_in &addr);
	void listen(int sockfd);
	int accept(int sockfd, struct sockaddr_in *addr);

	ssize_t read(int sockfd, void *buf, size_t count);
	ssize_t write(int sockfd, const void *buf, size_t count);

	void close(int sockfd);
	void shutdownWrite(int sockfd);

	int getSocketError(int sockfd);

	void toIpPort(char *buf, size_t size, const struct sockaddr_in &addr);
	void toIp(char *buf, size_t size, const struct sockaddr_in &addr);
	void fromIpPort(const char *ip, uint16_t port, struct sockaddr_in *addr);


}
}

#endif /*_SAIL_SOCKET_UTIL_H*/
