#ifndef __HANDLE_SETTER_H_INCLUDED__
#define __HANDLE_SETTER_H_INCLUDED__

#include "TestItem.h"

class HandleSetter
{
public:
	void operator()(TestItem* itemPtr, const PriorityQueueHandle& handle) const;
};

#endif //__HANDLE_SETTER_H_INCLUDED__