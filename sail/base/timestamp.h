
#ifndef _SAIL_TIMESTAMP_H
#define _SAIL_TIMESTAMP_H

#include <stdint.h>
#include <string>

namespace sail
{

class Timestamp
{
public:
    Timestamp(): _microSecondsSinceEpoch(0)
    {
    }

    explicit Timestamp(int64_t microSecondsSinceEpoch);


    bool operator < (const Timestamp &rhs)
    {
        return microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
    }

    bool operator == (const Timestamp &rhs)
    {
        return microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
    }

    std::string toString() const;

    bool valid() const { return _microSecondsSinceEpoch > 0; }


    // for internal usage.
    int64_t microSecondsSinceEpoch() const { return _microSecondsSinceEpoch; }
    time_t secondsSinceEpoch() const
    {
        return static_cast<time_t>(_microSecondsSinceEpoch / kMicroSecondsPerSecond);
    }

    ///
    /// Get time of now.
    ///
    static Timestamp now();
    static Timestamp invalid();

    static const int kMicroSecondsPerSecond = 1000;


private:
    int64_t _microSecondsSinceEpoch;
};


inline double timeDifference(Timestamp high, Timestamp low)
{
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}


}

#endif /*_SAIL_TIMESTAMP_H*/


