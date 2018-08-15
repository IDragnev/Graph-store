#ifndef __NO_MEMORY_AVAILABLE_H_INCLUDED__
#define __NO_MEMORY_AVAILABLE_H_INCLUDED__

#include "Exception.h"

class NoMemoryAvailable : public Exception
{
public:
	NoMemoryAvailable() : Exception{ "No memory available" } {}
	virtual ~NoMemoryAvailable() override = default;
};

#endif //__NO_MEMORY_AVAILABLE_H_INCLUDED__