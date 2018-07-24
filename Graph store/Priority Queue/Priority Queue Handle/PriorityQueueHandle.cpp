#include "PriorityQueueHandle.h"


PriorityQueueHandle::PriorityQueueHandle() :
	value(INVALID_HANDLE_VALUE)
{
}


PriorityQueueHandle::PriorityQueueHandle(long value) :
	value(value)
{
}


bool PriorityQueueHandle::isValid() const
{
	return value != INVALID_HANDLE_VALUE;
}


PriorityQueueHandle::operator long() const
{
	return value;
}
