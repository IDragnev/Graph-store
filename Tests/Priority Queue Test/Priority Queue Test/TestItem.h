#ifndef __TEST_ITEM_H_INCLUDED__
#define __TEST_ITEM_H_INCLUDED__

#include "../../../Graph store/Priority Queue/Priority Queue Handle/PriorityQueueHandle.h"

class TestItem
{
public:
	TestItem(unsigned ID = 0);

	void setHandle(const PriorityQueueHandle& handle);
	const PriorityQueueHandle& getHandle() const;

	void setID(unsigned ID);
	unsigned getID() const;

private:
	unsigned id;
	PriorityQueueHandle handle;
};

#endif //__TEST_ITEM_H_INCLUDED__