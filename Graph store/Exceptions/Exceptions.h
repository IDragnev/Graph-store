#ifndef __EXCEPTIONS_H_INCLUDED__
#define __EXCEPTIONS_H_INCLUDED__

#include <stdexcept>

namespace IDragnev::GraphStore
{
	class Exception : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	class NoMemoryAvailable : public Exception
	{
	public:
		NoMemoryAvailable() : Exception{ "No memory available" } {}
	};
}

#endif //__EXCEPTIONS_H_INCLUDED__