
#include <sail/net/inetaddr.h>
#include <sail/net/socketutil.h>

#include <netdb.h>
#include <strings.h>  // bzero

using namespace sail;

InetAddr::InetAddr(uint16_t port)
{
	bzero(&_addr, sizeof _addr);
	_addr.sin_family = AF_INET;
  	_addr.sin_addr.s_addr = sockets::hostToNetwork32(INADDR_ANY);
  	_addr.sin_port = sockets::hostToNetwork16(port);
}

InetAddr::InetAddr(const char *ip, uint16_t port)
{
	bzero(&_addr, sizeof _addr);
  	sockets::fromIpPort(ip, port, &_addr);
}

uint16_t InetAddr::toPort() const
{
  	return sockets::networkToHost16(_addr.sin_port);
}



