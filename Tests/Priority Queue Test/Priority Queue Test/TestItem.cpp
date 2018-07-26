#include "TestItem.h"


TestItem::TestItem(unsigned ID) :
	id(ID),
	handle()
{
}


void TestItem::setHandle(const PriorityQueueHandle& handle)
{
	this->handle = handle;
}


const PriorityQueueHandle& TestItem::getHandle() const
{
	return handle;
}


void TestItem::setID(unsigned ID)
{
	id = ID;
}

unsigned TestItem::getID() const
{
	return id;
}


bool operator==(const TestItem& lhs, const TestItem& rhs)
{
	return lhs.id == rhs.id;
}


bool operator!=(const TestItem& lhs, const TestItem& rhs)
{
	return !(lhs == rhs);
}