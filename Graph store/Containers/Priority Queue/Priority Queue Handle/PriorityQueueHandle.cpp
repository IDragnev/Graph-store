#include "PriorityQueueHandle.h"
#include <assert.h>

namespace Containers
{
	PriorityQueueHandle::PriorityQueueHandle(int32_t value) :
		value{ value }
	{
		assert(value >= -1);
	}


	bool PriorityQueueHandle::isValid() const
	{
		return value != -1;
	}


	PriorityQueueHandle::operator int32_t() const
	{
		return value;
	}
}