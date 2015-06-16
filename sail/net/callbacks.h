
#ifndef _SAIL_CALLBACKS_H
#define _SAIL_CALLBACKS_H

#include <stddef.h>

namespace sail
{

class Buffer;
class TcpConnection;

typedef void TimerCallback();
typedef void ConnectionCallback(const TcpConnection*);
typedef void CloseCallback(const TcpConnection*);
typedef void WriteCompleteCallback(const TcpConnection*);
typedef void HighWaterMarkCallback(const TcpConnection*, size_t);

typedef void HighWaterMarkCallback(const TcpConnection*, Buffer*, Timestamp);

}

#endif /*_SAIL_CALLBACKS_H*/
