#include "PriorityQueueHandle.h"


PriorityQueueHandle::PriorityQueueHandle() :
	PriorityQueueHandle(INVALID_HANDLE_VALUE)
{
}


PriorityQueueHandle::PriorityQueueHandle(long value) :
	value(value)
{
}


const PriorityQueueHandle& PriorityQueueHandle::invalidHandle()
{
	static PriorityQueueHandle handle(INVALID_HANDLE_VALUE);

	return handle;
}


bool PriorityQueueHandle::isValid() const
{
	return value != INVALID_HANDLE_VALUE;
}


PriorityQueueHandle::operator long() const
{
	return value;
}
