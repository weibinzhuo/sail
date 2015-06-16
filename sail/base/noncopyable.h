
#ifndef _SAIL_NON_COPYABLE_H
#define _SAIL_NON_COPYABLE_H

namespace sail
{

class NonCopyable
{
public:
    NonCopyable();
    ~NonCopyable();

private:
	NonCopyable(const NonCopyable&);
	const NonCopyable& operator=(const NonCopyable&);
};

}

#endif /*_SAIL_NON_COPYABLE_H*/


