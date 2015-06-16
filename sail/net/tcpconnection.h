
#ifndef _SAIL_TCP_CONNECTION_H
#define _SAIL_TCP_CONNECTION_H

#include <sail/base/noncopyable.h>
#include <sail/net/callbacks.h>
#include <sail/net/inetaddr.h>

#include <string>

namespace sail
{

class Channel;
class EventLoop;
class Socket;

class TcpConnection : public sail::NonCopyable
{
public:
	/* Constructs a TcpConnection with a connected sockfd
  	   User should not create this object.
  	 */
  	TcpConnection(EventLoop *loop,
                const std::string &name,
                int sockfd,
                const InetAddr &localAddr,
                const InetAddr &peerAddr);
  	~TcpConnection();

  	EventLoop* getLoop() const { return _loop; }
  	const std::string& name() const { return _name; }
  	const InetAddr& localAddr() const { return _localAddr; }
  	const InetAddr& peerAddr() const { return _peerAddr; }
  	bool connected() const { return _state == kConnected; }

  	void send(const void *message, int len);
  	void send(Buffer *message);  // this one will swap data

  	void shutdown(); // NOT thread safe, no simultaneous calling

  	void forceClose();
  	void forceCloseWithDelay(double seconds);
  	void setTcpNoDelay(bool on);

  	Buffer* inputBuffer() const { return &_inputBuffer; }
 	Buffer* outputBuffer() const { return &_outputBuffer; }

 	void setConnectionCallback(const ConnectionCallback *cb)
 	{ _connectionCallback = cb; }

  	void setMessageCallback(const MessageCallback *cb)
  	{ _messageCallback = cb; }

  	void setWriteCompleteCallback(const WriteCompleteCallback *cb)
  	{ _writeCompleteCallback = cb; }

  	void setHighWaterMarkCallback(const HighWaterMarkCallback *cb, size_t highWaterMark)
  	{
  		_highWaterMarkCallback = cb;
  		_highWaterMark = highWaterMark;
  	}

  	// Internal use only.
  	void setCloseCallback(const CloseCallback *cb)
  	{ _closeCallback = cb; }

private:
	enum StateE {
		kDisconnected,
		kConnecting,
		kConnected,
		kDisconnecting
	};

	EventLoop *_loop;
  	const std::string _name;
  	StateE _state;  // FIXME: use atomic variable

	// we don't expose those classes to client.
	Socket *_socket;
	Channel *_channel;
	const InetAddr _localAddr;
	const InetAddr _peerAddr;

	ConnectionCallback *_connectionCallback;
	MessageCallback *_messageCallback;
	WriteCompleteCallback *_writeCompleteCallback;
	HighWaterMarkCallback *_highWaterMarkCallback;
	CloseCallback *_closeCallback;

	size_t _highWaterMark;
	Buffer _inputBuffer;
	Buffer _outputBuffer; // FIXME: use list<Buffer> as output buffer.

	int _creationTime;
	int _lastReceiveTime;
	unsigned long _bytesReceived;
	unsigned long _byteSend;
};

}

#endif /*_SAIL_TCP_CONNECTION_H*/


