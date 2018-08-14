#ifndef __MY_EXCEPTION_H_INCLUDED__
#define __MY_EXCEPTION_H_INCLUDED__

#include <stdexcept>

template <typename T>
class Exception : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
	virtual ~Exception() override = default;
};

#endif //__MY_EXCEPTION_H_INCLUDED__