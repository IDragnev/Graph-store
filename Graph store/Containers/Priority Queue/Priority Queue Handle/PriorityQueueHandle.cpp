#include "PriorityQueueHandle.h"
#include <assert.h>

namespace IDragnev
{
	namespace Containers
	{
		PriorityQueueHandle::PriorityQueueHandle(std::int32_t value) :
			value{ value }
		{
			assert(value >= -1);
		}

		bool PriorityQueueHandle::isValid() const
		{
			return value != -1;
		}

		PriorityQueueHandle::operator std::int32_t() const
		{
			return value;
		}
	}
}