
#ifndef _SAIL_MUTEX_LOCK_H
#define _SAIL_MUTEX_LOCK_H

#include <pthread.h>

namespace sail
{

class MutexLock
{
public:
	MutexLock()
	{
		pthread_mutex_init(&_mutex, NULL);
	}

	~MutexLock()
	{
		pthread_mutex_destroy(&_mutex);
	}

	void lock()
	{
		pthread_mutex_lock(&_mutex);
	}

	void unlock()
	{
		pthread_mutex_unlock(&_mutex);
	}

private:
	// uncopyable class
	MutexLock(const MutexLock&);
	const MutexLock& operator=(const MutexLock&);

private:
	pthread_mutex_t _mutex;
};

// Use as a stack variable, eg.
// int Foo::size() const
// {
//   MutexLockGuard lock(_mutex);
//   return _data.size();
// }
class MutexLockGuard
{
public:
	explicit MutexLockGuard(MutexLock &mutex): _mutex(mutex)
	{
		_mutex.lock();
	}

	~MutexLockGuard()
	{
		_mutex.unlock();
	}

private:
	MutexLockGuard(const MutexLockGuard&);
	const MutexLockGuard& operator=(const MutexLockGuard&);

private:
	MutexLock& _mutex;
};

// Prevent misuse like:
// MutexLockGuard(_mutex);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"

}

#endif /*_SAIL_MUTEX_LOCK_H*/


