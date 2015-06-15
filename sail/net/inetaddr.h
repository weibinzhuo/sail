
#ifndef _SAIL_INET_ADDR_H
#define _SAIL_INET_ADDR_H

#include <netinet/in.h>

namespace sail
{

class InetAddr
{
public:
	explicit InetAddr(uint16_t port = 0);

	InetAddr(char *ip, uint16_t port);

	InetAddr(const struct sockaddr_in &addr)
		: _addr(addr)
	{ }

	uint16_t toPort() const;

	const struct sockaddr_in& getSockAddrInet() const { return _addr; }
	void setSockAddrInet(const struct sockaddr_in &addr) { _addr = addr; }

	uint32_t ipNetEndian() const { return _addr.sin_addr.s_addr; }
  	uint16_t portNetEndian() const { return _addr.sin_port; }

private:
	struct sockaddr_in _addr;
};

}

#endif /*_SAIL_INET_ADDR_H*/
