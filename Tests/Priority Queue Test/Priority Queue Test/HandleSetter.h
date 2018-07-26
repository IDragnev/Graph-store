#ifndef __HANDLE_SETTER_H_INCLUDED__
#define __HANDLE_SETTER_H_INCLUDED__

#include "TestItem.h"

class HandleSetter
{
public:
	void operator()(TestItem& item, const PriorityQueueHandle& handle) const
	{
		item.setHandle(handle);
	}
};

#endif //__HANDLE_SETTER_H_INCLUDED__