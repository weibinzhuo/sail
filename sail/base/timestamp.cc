
#include <sail/base/timestamp.h>
#include <sys/time.h>

using namespace sail;

Timestamp::Timestamp(int64_t microseconds)
  : _microSecondsSinceEpoch(microseconds)
{
}

std::string Timestamp::toString() const
{
    char buf[32] = {0};
    int64_t seconds = _microSecondsSinceEpoch / kMicroSecondsPerSecond;
    int64_t microseconds = _microSecondsSinceEpoch % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf)-1, "%ld s %ld ms", seconds, microseconds);
    return buf;
}

Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

Timestamp Timestamp::invalid()
{
    return Timestamp();
}


