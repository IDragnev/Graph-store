#ifndef __NO_MEMORY_AVAILABLE_H_INCLUDED__
#define __NO_MEMORY_AVAILABLE_H_INCLUDED__

#include "Exception.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class NoMemoryAvailable : public Exception
		{
		public:
			NoMemoryAvailable() : Exception{ "No memory available" } {}
		};
	}
}

#endif //__NO_MEMORY_AVAILABLE_H_INCLUDED__