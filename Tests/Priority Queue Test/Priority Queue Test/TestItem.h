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

	friend bool operator==(const TestItem& lhs, const TestItem& rhs);

private:
	TestItem(const TestItem&) = delete;
	TestItem& operator=(const TestItem&) = delete;

private:
	static unsigned nextAvailableID;

private:
	const unsigned ID;
	PriorityQueueHandle handle;
};

#endif //__TEST_ITEM_H_INCLUDED__