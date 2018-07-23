#include "TestItem.h"

unsigned TestItem::nextAvailableID = 0;


TestItem::TestItem() :
	ID(++TestItem::nextAvailableID),
	handle()
{
}


TestItem::~TestItem()
{
	--TestItem::nextAvailableID;
}


void TestItem::setHandle(PriorityQueueHandle handle)
{
	this->handle = handle;
}


PriorityQueueHandle TestItem::getHandle() const
{
	return handle;
}


bool operator==(const TestItem& lhs, const TestItem& rhs)
{
	return lhs.ID == rhs.ID;
}