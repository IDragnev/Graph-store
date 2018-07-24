#ifndef __TEST_ITEM_H_INCLUDED__
#define __TEST_ITEM_H_INCLUDED__

#include "../../../Graph store/Priority Queue/Priority Queue Handle/PriorityQueueHandle.h"

class TestItem
{
public:
	TestItem();
	~TestItem();

	void setHandle(const PriorityQueueHandle& handle);
	const PriorityQueueHandle& getHandle() const;
	unsigned getID() const;

	friend bool operator==(const TestItem& lhs, const TestItem& rhs);

private:
	static unsigned nextAvailableID;

private:
	unsigned ID;
	PriorityQueueHandle handle;
};

#endif //__TEST_ITEM_H_INCLUDED__