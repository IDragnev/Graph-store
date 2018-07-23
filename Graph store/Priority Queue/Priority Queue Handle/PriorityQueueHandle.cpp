#include "PriorityQueueHandle.h"


PriorityQueueHandle::PriorityQueueHandle() :
	value(INVALID_HANDLE_VALUE)
{
}


PriorityQueueHandle::PriorityQueueHandle(long value) :
	value(value)
{
}


PriorityQueueHandle::operator long() const
{
	return value;
}
