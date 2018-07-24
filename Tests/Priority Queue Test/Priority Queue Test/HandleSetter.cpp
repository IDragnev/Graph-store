#include "HandleSetter.h"
#include <assert.h>


void HandleSetter::operator()(TestItem* itemPtr, const PriorityQueueHandle& handle) const
{
	assert(itemPtr);
	itemPtr->setHandle(handle);
}