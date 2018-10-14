#include "PriorityQueueHandle.h"
#include <assert.h>

namespace Containers
{
	PriorityQueueHandle::PriorityQueueHandle(long value) :
		value{ value }
	{
		assert(value >= -1);
	}


	bool PriorityQueueHandle::isValid() const
	{
		return value != -1;
	}


	PriorityQueueHandle::operator const long() const
	{
		return value;
	}
}