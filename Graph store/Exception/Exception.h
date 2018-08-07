#ifndef __MY_EXCEPTION_H_INCLUDED__
#define __MY_EXCEPTION_H_INCLUDED__

#include <stdexcept>

template <typename T>
class Exception : public std::runtime_error
{
public:
	Exception(const char* message);
	Exception(const std::string& message);
	virtual ~Exception() override = default;
};

#include "ExceptionImpl.hpp"
#endif //__MY_EXCEPTION_H_INCLUDED__