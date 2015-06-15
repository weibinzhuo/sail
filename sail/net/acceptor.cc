
#include <sail/net/acceptor.h>
#include <sail/net/socketutil.h>
#include <sail/net/inetaddr.h>

#include <errno.h>
#include <fcntl.h>

using namespace sail;

Acceptor::Acceptor(EventLoop* loop, const InetAddr &listenAddr, bool reuseport)
	: _loop(loop),
	  _acceptSocket(sockets::createNonblocking()),
      _acceptChannel(loop, _acceptSocket.fd()),
      _newConnectionCallback(NULL),
      _listenning(false),
      _idleFd(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
	assert(_idleFd >= 0);
  	_acceptSocket.setReuseAddr(true);
  	_acceptSocket.setReusePort(reuseport);
  	_acceptSocket.bindAddress(listenAddr);
  	_acceptChannel.setReadCallback(&Acceptor::handleRead);
}

Acceptor::~Acceptor()
{
	_acceptChannel.disableAll();
  	_acceptChannel.remove();
  	::close(_idleFd);
}

void Acceptor::listen()
{
  	_listenning = true;
  	_acceptSocket.listen();
  	_acceptChannel.enableReading();
}

void Acceptor::handleRead()
{
  	InetAddr peerAddr;
  	int connfd = _acceptSocket.accept(&peerAddr);
  	if (connfd >= 0)
  	{
    // string hostport = peerAddr.toIpPort();
    // LOG_TRACE << "Accepts of " << hostport;
    	if (_newConnectionCallback)
    	{
      		_newConnectionCallback(connfd, peerAddr);
    	}
    	else
    	{
      		sockets::close(connfd);
    	}
  	}
  	else
  	{
    	LOG_SYSERR << "in Acceptor::handleRead";
    	// Read the section named "The special problem of
    	// accept()ing when you can't" in libev's doc.
    	// By Marc Lehmann, author of livev.
    	if (errno == EMFILE)
    	{
	      	::close(_idleFd);
	      	_idleFd = ::accept(_acceptSocket.fd(), NULL, NULL);
	      	::close(_idleFd);
	      	_idleFd = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
    	}
  	}
}
