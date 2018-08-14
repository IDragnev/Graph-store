#include "PriorityQueueHandle.h"
#include <assert.h>


PriorityQueueHandle::PriorityQueueHandle(long value) :
	value(value)
{
	assert(value >= -1);
}


const PriorityQueueHandle& PriorityQueueHandle::invalidHandle()
{
	static PriorityQueueHandle handle;

	return handle;
}


bool PriorityQueueHandle::isValid() const
{
	return value != -1;
}


PriorityQueueHandle::operator long() const
{
	return value;
}
